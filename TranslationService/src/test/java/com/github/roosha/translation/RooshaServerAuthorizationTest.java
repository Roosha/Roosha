/*
 *  Copyright (c) 2016 Leo Khotov
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  version 3 along with this work.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

package com.github.roosha.translation;

import com.github.roosha.proto.commons.CommonsProto.AuthenticationToken;
import com.github.roosha.proto.commons.CommonsProto.Credentials;
import com.github.roosha.proto.translation.RooshaTranslationServiceGrpc;
import com.github.roosha.proto.translation.RooshaTranslationServiceGrpc.RooshaTranslationServiceBlockingStub;
import com.github.roosha.proto.translation.TranslationServiceProto.TranslationRequest;
import com.github.roosha.proto.translation.TranslationServiceProto.Translations;
import com.github.roosha.proto.translation.TranslationServiceProto.UserTranslationsProposal;
import com.github.roosha.server.ErrorsStatusExceptions;
import com.github.roosha.server.RooshaServer;
import com.github.roosha.server.config.Config;
import com.github.roosha.server.util.UuidUtil;
import com.google.protobuf.ByteString;
import io.grpc.Channel;
import io.grpc.StatusRuntimeException;
import io.grpc.netty.GrpcSslContexts;
import io.grpc.netty.NettyChannelBuilder;
import io.netty.handler.ssl.util.InsecureTrustManagerFactory;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import javax.net.ssl.SSLException;
import java.io.IOException;
import java.security.SecureRandom;
import java.util.concurrent.Callable;

import static com.github.roosha.server.ErrorsStatusExceptions.authorizationFailure;
import static com.github.roosha.server.ErrorsStatusExceptions.registrationFailure;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

@SuppressWarnings("ThrowableResultOfMethodCallIgnored")
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = Config.class)
public class RooshaServerAuthorizationTest {
    private RooshaTestClient client;

    @Autowired
    private RooshaServer server;

    @Before
    public void setUp() throws IOException {
        server.start();
        client = new RooshaTestClient();
    }

    @After
    public void shutDown() {
        server.stop();
        client = null;
    }

    @Test
    public void invalidTokenPassCausesExpiredTokenException() {
        for (String source : new String[]{"source", "exhibit", "apple", "noise"}) {
            final TranslationRequest notEmptyTokenRequest =
                    TranslationRequest.newBuilder()
                                      .setSource(source)
                                      .setToken(UuidUtil.generateUuidASByteString())
                                      .build();
            final TranslationRequest emptyTokenRequest =
                    TranslationRequest.newBuilder().setSource(source).build();
            final TranslationRequest requestWithIdSet =
                    TranslationRequest.newBuilder().setSource(source).setId(new SecureRandom().nextLong()).build();

            assertAppropriateStatusRuntimeExceptionThrown(
                    () -> client.translate(notEmptyTokenRequest),
                    StatusRuntimeException.class,
                    ErrorsStatusExceptions.expiredAuthToken().getMessage()
            );
            assertAppropriateStatusRuntimeExceptionThrown(
                    () -> client.translate(emptyTokenRequest),
                    StatusRuntimeException.class,
                    ErrorsStatusExceptions.expiredAuthToken().getMessage()
            );
            assertAppropriateStatusRuntimeExceptionThrown(
                    () -> client.translate(requestWithIdSet),
                    StatusRuntimeException.class,
                    ErrorsStatusExceptions.expiredAuthToken().getMessage()
            );
        }
    }

    @Test
    public void registrationGeneratedValidToken() {
        for (String login : new String[]{"gylo", "anykuzya", "adkosm", "sandello"}) {
            final Credentials credentials =
                    Credentials.newBuilder()
                               .setLogin(login)
                               .setPasswordHash(UuidUtil.generateUuidASByteString())
                               .build();
            final ByteString token = client.register(credentials).getToken();

            final TranslationRequest translationRequestWithToken =
                    TranslationRequest.newBuilder().setToken(token).setSource("exhibit").build();
            final Translations response = client.translate(translationRequestWithToken);
            assertEquals("Invalid response source", translationRequestWithToken.getSource(), response.getSource());
        }
    }

    @Test
    public void reregistrationAttemptCausesRegistrationException() {
        final Credentials initialRegistrationRequest =
                Credentials.newBuilder()
                           .setLogin("anykuzya48")
                           .setPasswordHash(UuidUtil.generateUuidASByteString())
                           .build();
        client.register(initialRegistrationRequest);

        for (String login : new String[]{"Anykuzya48", "anYkuzya48", "AnYkUzYa48", "aNyKuZyA48", "ANYKUZYA48", "anykuzya48"}) {
            final Credentials credentialsWithTheSamePasswordHash =
                    initialRegistrationRequest.toBuilder().setLogin(login).build();
            final Credentials credentialsWithDifferentPasswordHash =
                    Credentials.newBuilder()
                               .setLogin(login)
                               .setPasswordHash(UuidUtil.generateUuidASByteString())
                               .build();
            assertAppropriateStatusRuntimeExceptionThrown(
                    () -> client.register(credentialsWithDifferentPasswordHash),
                    registrationFailure().getClass(),
                    registrationFailure().getMessage()
            );
            assertAppropriateStatusRuntimeExceptionThrown(
                    () -> client.register(credentialsWithTheSamePasswordHash),
                    registrationFailure().getClass(),
                    registrationFailure().getMessage()
            );
        }
    }

    @Test
    public void authorizationOfUnregisteredUserCausesAuthorizationException() {
        final Credentials validCredentials =
                Credentials.newBuilder()
                           .setLogin("registeredLogin")
                           .setPasswordHash(UuidUtil.generateUuidASByteString())
                           .build();
        client.register(validCredentials);
        final Credentials invalidPasswordCreds =
                validCredentials.toBuilder().setPasswordHash(UuidUtil.generateUuidASByteString()).build();
        final Credentials invalidLoginCreds = validCredentials.toBuilder().setLogin("notRegisteredLogin").build();
        final Credentials fullyInvalidCreds = Credentials.newBuilder()
                                                    .setPasswordHash(UuidUtil.generateUuidASByteString())
                                                    .setLogin("invalidLogin")
                                                    .build();

        assertAppropriateStatusRuntimeExceptionThrown(
                () -> client.authorize(invalidPasswordCreds),
                authorizationFailure().getClass(),
                authorizationFailure().getMessage()
        );
        assertAppropriateStatusRuntimeExceptionThrown(
                () -> client.authorize(invalidLoginCreds),
                authorizationFailure().getClass(),
                authorizationFailure().getMessage()
        );
        assertAppropriateStatusRuntimeExceptionThrown(
                () -> client.authorize(fullyInvalidCreds),
                authorizationFailure().getClass(),
                authorizationFailure().getMessage()
        );
    }

    private void assertAppropriateStatusRuntimeExceptionThrown(
            Callable testJob,
            Class<? extends Throwable> expectedExceptionClass,
            String expectedMessage) {
        boolean exceptionThrown = false;
        String resultGotten = null;
        try {
            resultGotten = testJob.call().toString();
        } catch (Throwable e) {
            exceptionThrown = true;

            assertTrue(
                    "Unexpected excpetion type " + e.getClass().getSimpleName() + ". " +
                            expectedExceptionClass.getSimpleName() + " expected",
                    e.getClass().equals(expectedExceptionClass)
            );
            assertEquals("Not expected message.", expectedMessage, e.getMessage());
        }
        assertTrue("No exception thrown, but reveived result:\n" + resultGotten, exceptionThrown);
    }
}

class RooshaTestClient {
    private final RooshaTranslationServiceBlockingStub blockingStub;
    private final Channel channel;

    public RooshaTestClient() throws SSLException {
        final String host = "127.0.0.1";
        final int port = 1543;


        this.channel = NettyChannelBuilder.forAddress(host, port)
                                          .sslContext(GrpcSslContexts.forClient()
                                                                     // .trustManager(getClass().getResourceAsStream("/security/ca.crt"))
                                                                     // Now, in test cases, we're using self signed SSL sertificate on the Server, thereby we disable sertificate validation.
                                                                     .trustManager(InsecureTrustManagerFactory.INSTANCE)
                                                                     .build())
                                          .build();
        blockingStub = RooshaTranslationServiceGrpc.newBlockingStub(channel);
    }

    AuthenticationToken authorize(Credentials credentials) throws StatusRuntimeException {
        return blockingStub.authorize(credentials);
    }

    AuthenticationToken register(Credentials credentials) throws StatusRuntimeException {
        return blockingStub.register(credentials);
    }

    Translations translate(TranslationRequest request) throws StatusRuntimeException {
        return blockingStub.translate(request);
    }

    void poposeUserTranslations(UserTranslationsProposal proposal) throws StatusRuntimeException {
        blockingStub.proposeUserTranslations(proposal);
    }
}