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
import com.github.roosha.proto.translation.TranslationServiceProto.UserTranslationsProposal;
import com.github.roosha.server.auth.AuthorizationManager;
import com.github.roosha.server.auth.RequireAuthentication;
import com.github.roosha.server.translation.providers.RawTranslation;
import com.github.roosha.server.translation.providers.TranslationProvider;
import com.google.protobuf.ByteString;
import io.grpc.stub.StreamObserver;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import static com.github.roosha.proto.commons.CommonsProto.*;
import static com.github.roosha.proto.commons.CommonsProto.Void;

@Component
public class RooshaRpcService extends RooshaServiceImplBase {
    @Autowired
    private TranslationProvider provider;

    @Autowired
    private AuthorizationManager authManager;

    @Override
    @RequireAuthentication(request = RequireAuthentication.RequestType.TRANSLATE)
    public void translate(TranslationRequest request, StreamObserver<Translations> responseObserver) {
        try {
            final RawTranslation rawTranslation = provider.translate(request.getSource());
            final Translations.Builder responseBuilder =
                    rawTranslation.convertToProtoTranslationsBuilder();
            if (responseBuilder != null) {
                rawTranslation.addToProtoTranslationsBuilder(responseBuilder);
                responseBuilder.setSource(request.getSource());
            }
            else {
                throw ErrorsStatusExceptions.noTranslation(request.getSource());
            }
            responseObserver.onNext(responseBuilder.build());
            responseObserver.onCompleted();
        } catch (Throwable t) {
            responseObserver.onError(t);
        }
    }

    @Override
    @RequireAuthentication(request = RequireAuthentication.RequestType.PROPOSE_USER_TRANSLATIONS)
    public void proposeUserTranslations(UserTranslationsProposal request, StreamObserver<Void> responseObserver) {
        responseObserver.onNext(Void.getDefaultInstance());
        responseObserver.onCompleted();
    }

    @Override
    public void registrate(Credentials request, StreamObserver<AuthenticationToken> responseObserver) {
        try {
            final ByteString authToken = authManager.register(request);
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
            final ByteString authToken = authManager.authorize(request);
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
}
