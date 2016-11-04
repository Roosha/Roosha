package com.github.roosha.server.auth;

import io.grpc.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

import static com.github.roosha.server.util.Assertions.assertNotNull;

@Component
public class AuthInterceptor implements ServerInterceptor {
    private final Context.Key<Long> ID_CONTEXT_KEY;
    private final Metadata.Key<String> TOKEN_METADATA_KEY;
    private final AuthorizationManager authorizationManager;

    @Autowired
    private AuthInterceptor(
            @Qualifier("authorizedUserIdContextKey") Context.Key<Long> ID_CONTEXT_KEY,
            AuthorizationManager authorizationManager,
            @Qualifier("authTokenMetadataKey") Metadata.Key<String> TOKEN_METADATA_KEY) {
        assertNotNull(ID_CONTEXT_KEY, "id context key must be not null");
        assertNotNull(authorizationManager, "authorization manager must be not null");
        assertNotNull(TOKEN_METADATA_KEY, "token metadata key must be not null");

        this.ID_CONTEXT_KEY = ID_CONTEXT_KEY;
        this.authorizationManager = authorizationManager;
        this.TOKEN_METADATA_KEY = TOKEN_METADATA_KEY;
    }

    @Override
    public <ReqT, RespT> ServerCall.Listener<ReqT> interceptCall(
            ServerCall<ReqT, RespT> call, Metadata headers, ServerCallHandler<ReqT, RespT> next) {
        final String token = headers.get(TOKEN_METADATA_KEY);
        long userId = -1;
        if (token != null) {
            userId = authorizationManager.getUserIdByToken(token);
        }

        final Context context = Context.current().withValue(ID_CONTEXT_KEY, userId);
        return Contexts.interceptCall(context, call, headers, next);
    }
}
