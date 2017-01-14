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
import com.github.roosha.server.auth.AuthManager;
import com.github.roosha.server.db.WorldStorage;
import com.github.roosha.server.translation.TranslationsCacheManager;
import com.github.roosha.server.translation.providers.RawTranslation;
import com.github.roosha.server.translation.providers.TranslationProvider;
import io.grpc.Context;
import io.grpc.StatusRuntimeException;
import io.grpc.stub.StreamObserver;
import lombok.val;
import org.jetbrains.annotations.NotNull;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
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
    private static final String RPC_COMPLETION_LOG_FMT_STRING = "{} RPC completed.\nRequest:{}\nResponse{}";
    private static final Logger log = LoggerFactory.getLogger(RooshaRpcService.class);

    private final TranslationsCacheManager cacheManager;
    private final TranslationProvider provider;
    private final AuthManager authManager;
    private final Context.Key<Long> USER_ID_CONTEXT_KEY;
    private final WorldStorage worldStorage;

    @Autowired
    public RooshaRpcService(
            TranslationsCacheManager cacheManager,
            TranslationProvider provider,
            AuthManager authManager,
            @Qualifier("authorizedUserIdContextKey") Context.Key<Long> USER_ID_CONTEXT_KEY,
            WorldStorage worldStorage) {
        assertNotNull(cacheManager, "cache manager should not be null");
        assertNotNull(provider, "provider should not be null");
        assertNotNull(authManager, "authorization manager should not be null");
        assertNotNull(USER_ID_CONTEXT_KEY, "userId context key should not be null");
        assertNotNull(worldStorage, "world worldStorage should not be null");

        this.cacheManager = cacheManager;
        this.provider = provider;
        this.authManager = authManager;
        this.USER_ID_CONTEXT_KEY = USER_ID_CONTEXT_KEY;
        this.worldStorage = worldStorage;
    }


    @Override
    public void translate(TranslationRequest request, StreamObserver<Translations> responseObserver) {
        requireCallerUserId();
        Translations response = cacheManager.translate(request.getSource());
        if (response == null) {
            response = translate(request.getSource());
            cacheManager.save(response);
        }

        responseObserver.onNext(response);
        responseObserver.onCompleted();

        log.debug(RPC_COMPLETION_LOG_FMT_STRING, "translate", request, response);
    }

    @Override
    public void proposeUserTranslations(Translations request, StreamObserver<Void> responseObserver) {
        requireCallerUserId();

        cacheManager.save(request);

        val response = Void.getDefaultInstance();
        responseObserver.onNext(response);
        responseObserver.onCompleted();

        log.debug(RPC_COMPLETION_LOG_FMT_STRING, "proposeUserTranslations", request, response);
    }

    @Override
    public void registrate(Credentials request, StreamObserver<AuthenticationToken> responseObserver) {
        final String authToken = authManager.register(request);
        if (authToken != null) {
            val response = AuthenticationToken.newBuilder().setToken(authToken).build();
            responseObserver.onNext(response);
            responseObserver.onCompleted();

            log.debug(RPC_COMPLETION_LOG_FMT_STRING, "registrate", request, response);
        } else {
            throw ErrorsStatusExceptions.registrationFailure();
        }

    }

    @Override
    public void authorize(Credentials request, StreamObserver<AuthenticationToken> responseObserver) {
        final String authToken = authManager.authorize(request);
        if (authToken != null) {
            val response = AuthenticationToken.newBuilder().setToken(authToken).build();
            responseObserver.onNext(response);
            responseObserver.onCompleted();

            log.debug(RPC_COMPLETION_LOG_FMT_STRING, "authorize", request, response);
        } else {
            throw ErrorsStatusExceptions.authorizationFailure();
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

                responseObserver.onNext(Void.getDefaultInstance());
                responseObserver.onCompleted();
            }
        };
    }

    @Override
    public void loadChanges(
            Void request, StreamObserver<Change> responseObserver) {
        final long userId = requireCallerUserId();

        final List<Change> world = worldStorage.getWorld(userId);
        if (world != null) {
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

    private @NotNull Translations translate(@NotNull String source) {
        final RawTranslation rawTranslation = provider.translate(source);
        Translations.Builder responseBuilder = rawTranslation.convertToProtoTranslationsBuilder();
        if (responseBuilder == null) {
            responseBuilder = Translations.newBuilder(Translations.getDefaultInstance());
        }
        return responseBuilder.build();
    }
}
