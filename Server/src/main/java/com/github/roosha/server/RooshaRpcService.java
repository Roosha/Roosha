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

package com.github.roosha.server;

import com.github.roosha.proto.translation.RooshaServiceGrpc.RooshaServiceImplBase;
import com.github.roosha.proto.translation.TranslationServiceProto.TranslationRequest;
import com.github.roosha.proto.translation.TranslationServiceProto.Translations;
import com.github.roosha.server.auth.AuthorizationManager;
import com.github.roosha.server.translation.providers.RawTranslation;
import com.github.roosha.server.translation.providers.TranslationProvider;
import io.grpc.Context;
import io.grpc.StatusRuntimeException;
import io.grpc.stub.StreamObserver;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

import static com.github.roosha.proto.commons.CommonsProto.*;
import static com.github.roosha.proto.commons.CommonsProto.Void;
import static com.github.roosha.server.ErrorsStatusExceptions.expiredAuthToken;
import static com.github.roosha.server.util.Assertions.assertNotNull;

@Component
public class RooshaRpcService extends RooshaServiceImplBase {
    private final TranslationProvider provider;

    private final AuthorizationManager authManager;

    private final Context.Key<Long> USER_ID_CONTEXT_KEY;

    @Autowired
    public RooshaRpcService(
            TranslationProvider provider,
            AuthorizationManager authManager,
            @Qualifier("authorizedUserIdContextKey") Context.Key<Long> USER_ID_CONTEXT_KEY) {
        assertNotNull(provider, "provider must not be null");
        assertNotNull(authManager, "authorization manager must not be null");
        assertNotNull(USER_ID_CONTEXT_KEY, "userId context key must not be null");

        this.provider = provider;
        this.authManager = authManager;
        this.USER_ID_CONTEXT_KEY = USER_ID_CONTEXT_KEY;
    }


    @Override
    public void translate(TranslationRequest request, StreamObserver<Translations> responseObserver) {
        System.err.println("translate called");
//        requireCallerUserId();

        final RawTranslation rawTranslation = provider.translate(request.getSource());
        final Translations.Builder responseBuilder = rawTranslation.convertToProtoTranslationsBuilder();
        if (responseBuilder != null) {
            rawTranslation.addToProtoTranslationsBuilder(responseBuilder);
            responseBuilder.setSource(request.getSource());
        } else {
            throw ErrorsStatusExceptions.noTranslation(request.getSource());
        }
        responseObserver.onNext(responseBuilder.build());
        responseObserver.onCompleted();
    }

    @Override
    public void proposeUserTranslations(Translations request, StreamObserver<Void> responseObserver) {
        requireCallerUserId();

        responseObserver.onNext(Void.getDefaultInstance());
        responseObserver.onCompleted();
    }

    @Override
    public void registrate(Credentials request, StreamObserver<AuthenticationToken> responseObserver) {
        try {
            final String authToken = authManager.register(request);
            if (authToken != null) {
                responseObserver.onNext(AuthenticationToken.newBuilder().setToken(authToken).build());
                responseObserver.onCompleted();
            } else {
                throw ErrorsStatusExceptions.registrationFailure();
            }
        } catch (Throwable t) {
            responseObserver.onError(t);
        }
    }

    @Override
    public void authorize(Credentials request, StreamObserver<AuthenticationToken> responseObserver) {
        try {
            final String authToken = authManager.authorize(request);
            if (authToken != null) {
                responseObserver.onNext(AuthenticationToken.newBuilder().setToken(authToken).build());
                responseObserver.onCompleted();
            } else {
                throw ErrorsStatusExceptions.authorizationFailure();
            }
        } catch (Throwable t) {
            responseObserver.onError(t);
        }
    }

    /**
     * Extract userId of caller if call is authorized, throw {@link ErrorsStatusExceptions#expiredAuthToken()} otherwise.
     */
    private long requireCallerUserId() throws StatusRuntimeException {
        final long userId = USER_ID_CONTEXT_KEY.get();
        if (userId == -1) {
            throw expiredAuthToken();
        }

        return userId;
    }
}
