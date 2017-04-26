package com.github.roosha.server;

import io.grpc.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

import static com.github.roosha.server.util.Assertions.assertNotNull;

/**
 * Created by anna on 05.04.17.
 */
@Component
public class SynchronizationInterceptor implements ServerInterceptor {

    private final Context.Key<Long> HISTORY_LEN_CONTEXT_KEY;
    private final Metadata.Key<String> HISTORY_LEN_METADATA_KEY;

    @Autowired
    private SynchronizationInterceptor(
            @Qualifier("historyLenContextKey") Context.Key<Long> HISTORY_LEN_CONTEXT_KEY,
            @Qualifier("historyLenMetadataKey") Metadata.Key<String> HISTORY_LEN_METADATA_KEY) {
        assertNotNull(HISTORY_LEN_CONTEXT_KEY, "len key must be not null");
        assertNotNull(HISTORY_LEN_METADATA_KEY, "len metadata key must be not null");

        this.HISTORY_LEN_CONTEXT_KEY = HISTORY_LEN_CONTEXT_KEY;
        this.HISTORY_LEN_METADATA_KEY= HISTORY_LEN_METADATA_KEY;
    }
    @Override
    public <ReqT, RespT> ServerCall.Listener<ReqT> interceptCall(
            ServerCall<ReqT, RespT> call, Metadata headers, ServerCallHandler<ReqT, RespT> next) {

        final String strHistoryLen = headers.get(HISTORY_LEN_METADATA_KEY);
        long historyLen = -1;
        if (strHistoryLen != null) {
            historyLen = Long.parseLong(strHistoryLen);
        }
        final Context context = Context.current().withValue(HISTORY_LEN_CONTEXT_KEY, historyLen);
        return Contexts.interceptCall(context, call, headers, next);
    }
}
