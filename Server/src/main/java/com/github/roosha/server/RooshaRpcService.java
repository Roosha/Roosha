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

import com.github.roosha.proto.ChangesProto.Change;
import com.github.roosha.proto.RooshaServiceGrpc.RooshaServiceImplBase;
import com.github.roosha.proto.TranslationServiceProto.TranslationRequest;
import com.github.roosha.proto.TranslationServiceProto.Translations;
import com.github.roosha.server.auth.AuthorizationManager;
import com.github.roosha.server.db.WorldStorage;
import com.github.roosha.server.translation.providers.RawTranslation;
import com.github.roosha.server.translation.providers.TranslationProvider;
import io.grpc.Context;
import io.grpc.StatusRuntimeException;
import io.grpc.stub.StreamObserver;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

import java.util.LinkedList;
import java.util.List;

import static com.github.roosha.proto.CommonsProto.*;
import static com.github.roosha.proto.CommonsProto.Void;
import static com.github.roosha.server.ErrorsStatusExceptions.expiredAuthToken;
import static com.github.roosha.server.util.Assertions.assertNotNull;

@Component
public class RooshaRpcService extends RooshaServiceImplBase {
    private final TranslationProvider provider;
    private final AuthorizationManager authManager;
    private final Context.Key<Long> USER_ID_CONTEXT_KEY;
    private final WorldStorage worldStorage;

    @Autowired
    public RooshaRpcService(
            TranslationProvider provider,
            AuthorizationManager authManager,
            @Qualifier("authorizedUserIdContextKey") Context.Key<Long> USER_ID_CONTEXT_KEY,
            WorldStorage worldStorage) {
        assertNotNull(provider, "provider should not be null");
        assertNotNull(authManager, "authorization manager should not be null");
        assertNotNull(USER_ID_CONTEXT_KEY, "userId context key should not be null");
        assertNotNull(worldStorage, "world worldStorage should not be null");

        this.provider = provider;
        this.authManager = authManager;
        this.USER_ID_CONTEXT_KEY = USER_ID_CONTEXT_KEY;
        this.worldStorage = worldStorage;
    }


    @Override
    public void translate(TranslationRequest request, StreamObserver<Translations> responseObserver) {
        requireCallerUserId();

        final RawTranslation rawTranslation = provider.translate(request.getSource());
        Translations.Builder responseBuilder = rawTranslation.convertToProtoTranslationsBuilder();
        if (responseBuilder != null) {
            rawTranslation.addToProtoTranslationsBuilder(responseBuilder);
            responseBuilder.setSource(request.getSource());
        } else {
            responseBuilder = Translations.newBuilder(Translations.getDefaultInstance());
//            throw ErrorsStatusExceptions.noTranslation(request.getSource());
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

    @Override
    public StreamObserver<Change> saveChanges(StreamObserver<Void> responseObserver) {
        final long userId = requireCallerUserId();

        return new StreamObserver<Change>() {
            private final List<Change> world = new LinkedList<>();
            @Override
            public void onNext(Change value) {
                world.add(value);
            }

            @Override
            public void onError(Throwable t) {
                t.printStackTrace();
            }

            @Override
            public void onCompleted() {
                worldStorage.setWorld(userId, world);
            }
        };
    }

    @Override
    public void loadChanges(
            Void request, StreamObserver<Change> responseObserver) {
        final long userId = requireCallerUserId();

        final List<Change> world = worldStorage.getWorld(userId);
        if (world == null) {
            responseObserver.onNext(Change.getDefaultInstance());
        } else {
            world.forEach(responseObserver::onNext);
        }
        responseObserver.onCompleted();
    }

    /**
     * Extract userId of caller if call is authorized, throw {@link ErrorsStatusExceptions#expiredAuthToken()} otherwise.
     * @return valid id of authorized user who made the request
     */
    private long requireCallerUserId() throws StatusRuntimeException {
        final long userId = USER_ID_CONTEXT_KEY.get();
        if (userId == -1) {
            throw expiredAuthToken();
        }

        return userId;
    }
}
