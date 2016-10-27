package com.github.roosha.proto.translation;

import io.grpc.Metadata;
import io.grpc.ServerCall;
import io.grpc.ServerCallHandler;
import io.grpc.ServerInterceptor;

/**
 * This class is intended to be responsible for verification of user token.
 */
public class AuthorizationInterceptor implements ServerInterceptor {
    @Override
    public <ReqT, RespT> ServerCall.Listener<ReqT> interceptCall(
            ServerCall<ReqT, RespT> call, Metadata headers, ServerCallHandler<ReqT, RespT> next) {
        System.out.println("Intercepted call, received metadata: " + headers);
        final Metadata.Key<String> key = Metadata.Key.of("my_key", Metadata.ASCII_STRING_MARSHALLER);
        if (!headers.containsKey(key)) {
            System.err.println("No key found.");
        }
        else {
            System.out.println("Got metadata: " + headers.get(key));
        }

        return next.startCall(call, headers);
    }
}
