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

package com.github.roosha.server.auth;

import com.github.roosha.proto.CommonsProto.AuthenticationToken;
import com.github.roosha.proto.CommonsProto.Credentials;
import com.github.roosha.proto.RooshaServiceGrpc;
import com.github.roosha.proto.RooshaServiceGrpc.RooshaServiceBlockingStub;
import com.github.roosha.proto.TranslationServiceProto.TranslationRequest;
import com.github.roosha.proto.TranslationServiceProto.Translations;
import com.github.roosha.server.ErrorsStatusExceptions;
import com.github.roosha.server.RooshaServer;
import com.github.roosha.server.config.Config;
import io.grpc.*;
import io.grpc.netty.GrpcSslContexts;
import io.grpc.netty.NettyChannelBuilder;
import io.grpc.stub.MetadataUtils;
import io.netty.handler.ssl.util.InsecureTrustManagerFactory;
import org.junit.After;
import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import javax.net.ssl.SSLException;
import java.io.IOException;
import java.util.UUID;
import java.util.concurrent.Callable;

import static com.github.roosha.server.ErrorsStatusExceptions.authorizationFailure;
import static com.github.roosha.server.ErrorsStatusExceptions.registrationFailure;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

@SuppressWarnings("ThrowableResultOfMethodCallIgnored")
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = Config.class)
public class RooshaServerAuthorizationTest {
    @Autowired
    private RooshaServer server;

    @Autowired
    @Qualifier("authTokenMetadataKey")
    private Metadata.Key<String> TOKEN_METADATA_KEY;

    @Autowired
    @Qualifier("serverPort")
    private Integer port;

    @Before
    public void setUp() throws IOException {
        server.start();
    }

    @After
    public void shutDown() {
        server.stop();
    }

    @Ignore
    @Test
    public void invalidTokenPassCausesExpiredTokenException() {
        for (String source : new String[]{"source", "exhibit", "apple", "noise"}) {
            final TranslationRequest request = TranslationRequest.newBuilder().setSource(source).build();

            final RooshaTestClient clientWithNoToken = new RooshaTestClient();
            final RooshaTestClient clientWithRandomToken = new RooshaTestClient(UUID.randomUUID().toString());

            assertAppropriateStatusRuntimeExceptionThrown(
                    () -> clientWithNoToken.translate(request),
                    StatusRuntimeException.class,
                    ErrorsStatusExceptions.expiredAuthToken().getMessage()
            );
            assertAppropriateStatusRuntimeExceptionThrown(
                    () -> clientWithRandomToken.translate(request),
                    StatusRuntimeException.class,
                    ErrorsStatusExceptions.expiredAuthToken().getMessage()
            );
        }
    }

    @Ignore
    @Test
    public void registrationGeneratedValidToken() {
        for (String login : new String[]{"gylo", "anykuzya", "adkosm", "sandello"}) {
            final Credentials credentials =
                    Credentials.newBuilder()
                               .setLogin(login)
                               .setPasswordHash(UUID.randomUUID().toString())
                               .build();

            RooshaTestClient client = new RooshaTestClient();
            final String token = client.register(credentials).getToken();
            client = new RooshaTestClient(token);

            final TranslationRequest request = TranslationRequest.newBuilder().setSource("exhibit").build();
            final Translations response = client.translate(request);
            assertEquals("Invalid response source", request.getSource(), response.getSource());
        }
    }

    @Ignore
    @Test
    public void reregistrationAttemptCausesRegistrationException() {
        final Credentials initialRegistrationRequest =
                Credentials.newBuilder()
                           .setLogin("anykuzya48")
                           .setPasswordHash(UUID.randomUUID().toString())
                           .build();
        final RooshaTestClient client =
                new RooshaTestClient(new RooshaTestClient().register(initialRegistrationRequest).getToken());

        for (String login : new String[]{"Anykuzya48", "anYkuzya48", "AnYkUzYa48", "aNyKuZyA48", "ANYKUZYA48", "anykuzya48"}) {
            final Credentials credentialsWithTheSamePasswordHash =
                    initialRegistrationRequest.toBuilder().setLogin(login).build();
            final Credentials credentialsWithDifferentPasswordHash =
                    Credentials.newBuilder()
                               .setLogin(login)
                               .setPasswordHash(UUID.randomUUID().toString())
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

    @Ignore
    @Test
    public void authorizationOfUnregisteredUserCausesAuthorizationException() {
        final Credentials validCredentials =
                Credentials.newBuilder()
                           .setLogin("registeredLogin")
                           .setPasswordHash(UUID.randomUUID().toString())
                           .build();
        final RooshaTestClient client =
                new RooshaTestClient(new RooshaTestClient().register(validCredentials).getToken());
        final Credentials invalidPasswordCreds =
                validCredentials.toBuilder().setPasswordHash(UUID.randomUUID().toString()).build();
        final Credentials invalidLoginCreds = validCredentials.toBuilder().setLogin("notRegisteredLogin").build();
        final Credentials fullyInvalidCreds = Credentials.newBuilder()
                                                    .setPasswordHash(UUID.randomUUID().toString())
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


    class RooshaTestClient {
        private final RooshaServiceBlockingStub blockingStub;

        RooshaTestClient(){
            this(null);
        }

        RooshaTestClient(String token) {
            try {
                final String host = "127.0.0.1";
                Channel channel = NettyChannelBuilder.forAddress(host, port)
                                                     .sslContext(GrpcSslContexts.forClient()
                                                                                // .trustManager(getClass().getResourceAsStream("/security/ca.crt"))
                                                                                // Now, in test cases, we're using self signed SSL sertificate on the Server, thereby we disable sertificate validation.
                                                                                .trustManager(InsecureTrustManagerFactory.INSTANCE)
                                                                                .build())
                                                     .build();
                if (token != null) {
                    final Metadata tokenHeader = new Metadata();
                    tokenHeader.put(TOKEN_METADATA_KEY, token);
                    final ClientInterceptor interceptor = MetadataUtils.newAttachHeadersInterceptor(tokenHeader);
                    channel = ClientInterceptors.intercept(channel, interceptor);
                }

                blockingStub = RooshaServiceGrpc.newBlockingStub(channel);
            } catch (SSLException e) {
                throw new RuntimeException(e);
            }
        }

        AuthenticationToken authorize(Credentials credentials) throws StatusRuntimeException {
            return blockingStub.authorize(credentials);
        }

        AuthenticationToken register(Credentials credentials) throws StatusRuntimeException {
            return blockingStub.registrate(credentials);
        }

        Translations translate(TranslationRequest request) throws StatusRuntimeException {
            return blockingStub.translate(request);
        }

        void poposeUserTranslations(Translations proposal) throws StatusRuntimeException {
            blockingStub.proposeUserTranslations(proposal);
        }
    }
}