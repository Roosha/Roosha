package com.github.roosha.server.auth;

import com.github.roosha.proto.translation.TranslationServiceProto.TranslationRequest;
import com.github.roosha.proto.translation.TranslationServiceProto.UserTranslationsProposal;
import com.google.protobuf.ByteString;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.util.function.BiFunction;
import java.util.function.Function;

/**
 * Annotation used to mark methods to be woven with {@link AuthAspect}.
 */
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
public @interface RequireAuthentication {
    RequestType request();

    enum RequestType {
        TRANSLATE(
                request -> ((TranslationRequest) request).getToken(),
                (request, id) -> ((TranslationRequest) request).toBuilder().setId(id).build()
        ),
        PROPOSE_USER_TRANSLATIONS(
                request -> ((UserTranslationsProposal) request).getToken(),
                (request, id) -> ((UserTranslationsProposal) request).toBuilder().setId(id).build()
        ),
        ;

        private final Function<Object, ByteString> tokenGetter;
        private final BiFunction<Object, Long, Object> idSetter;

        RequestType(Function<Object, ByteString> tokenGetter, BiFunction<Object, Long, Object> idSetter) {
            this.tokenGetter = tokenGetter;
            this.idSetter = idSetter;
        }

        public ByteString getToken(Object request) {
            return tokenGetter.apply(request);
        }

        public Object withUserId(Object request, long userId) {
            return idSetter.apply(request, userId);
        }
    }
}
