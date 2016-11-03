package com.github.roosha.server.auth;

import com.github.roosha.server.ErrorsStatusExceptions;
import com.github.roosha.server.auth.RequireAuthentication.RequestType;
import com.github.roosha.server.config.SpringApplicationContextHolder;
import io.grpc.stub.StreamObserver;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.Around;
import org.aspectj.lang.annotation.Aspect;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.DependsOn;
import org.springframework.stereotype.Component;

@DependsOn("springApplicationContextHolder")
@Component
@Aspect
public class AuthAspect {
    @Autowired
    private AuthorizationManager authorizationManager;

    /**
     * <b>Magic!</b>
     * This method is used in order to enable Spring dependency injection via {@code @Autowired} for this aspect.
     * For more information, see <a href=http://stackoverflow.com/a/18822005/6632036>discussion at stackoverflow</a>.
     */
    public static AuthAspect aspectOf() {
        return SpringApplicationContextHolder.getApplicationContext().getBean(AuthAspect.class);
    }

    @Around(value = "@annotation(requireAuthentication) && args(request, responseObserver)")
    public void authorizeIfPossible(
            ProceedingJoinPoint joinPoint,
            RequireAuthentication requireAuthentication,
            Object request,
            StreamObserver responseObserver) throws Throwable {
        final RequestType requestTypeType = requireAuthentication.request();
        final long userId = authorizationManager.getUserIdByToken(requestTypeType.getToken(request));

        if (userId == -1) {
            responseObserver.onError(ErrorsStatusExceptions.expiredAuthToken());
        } else {
            joinPoint.proceed(new Object[]{requestTypeType.withUserId(request, userId), responseObserver});
        }
    }
}
