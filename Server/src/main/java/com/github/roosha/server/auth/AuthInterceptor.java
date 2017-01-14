package com.github.roosha.server.auth;

import io.grpc.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

import static com.github.roosha.server.util.Assertions.assertNotNull;

@Component
public class AuthInterceptor implements ServerInterceptor {
    private static final Logger log = LoggerFactory.getLogger(AuthInterceptor.class);

    private final Context.Key<Long> ID_CONTEXT_KEY;
    private final Metadata.Key<String> TOKEN_METADATA_KEY;
    private final AuthManager authManager;

    @Autowired
    private AuthInterceptor(
            @Qualifier("authorizedUserIdContextKey") Context.Key<Long> ID_CONTEXT_KEY,
            AuthManager authManager,
            @Qualifier("authTokenMetadataKey") Metadata.Key<String> TOKEN_METADATA_KEY) {
        assertNotNull(ID_CONTEXT_KEY, "id context key must be not null");
        assertNotNull(authManager, "authorization manager must be not null");
        assertNotNull(TOKEN_METADATA_KEY, "token metadata key must be not null");

        this.ID_CONTEXT_KEY = ID_CONTEXT_KEY;
        this.authManager = authManager;
        this.TOKEN_METADATA_KEY = TOKEN_METADATA_KEY;
    }

    @Override
    public <ReqT, RespT> ServerCall.Listener<ReqT> interceptCall(
            ServerCall<ReqT, RespT> call, Metadata headers, ServerCallHandler<ReqT, RespT> next) {
        final String token = headers.get(TOKEN_METADATA_KEY);
        long userId = -1;
        if (token != null) {
            userId = authManager.getUserIdByToken(token);
        }
        final Context context = Context.current().withValue(ID_CONTEXT_KEY, userId);

        log.debug("Successfully intercepted method {}. Received token {}. Id of user authorized with the token is {}",
                call.getMethodDescriptor().getFullMethodName(),
                token,
                userId);
        return Contexts.interceptCall(context, call, headers, next);
    }
}
