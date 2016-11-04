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

import io.grpc.Status;
import io.grpc.StatusRuntimeException;
import org.jetbrains.annotations.NotNull;

import static java.lang.String.format;

/**
 * This class provides default error statuses for translation service protocol.
 */
@SuppressWarnings("WeakerAccess")
public class ErrorsStatusExceptions {
    /**
     * Translation failure status exception for default translation direction.
     * @param source the text fail to be translated
     * @see ErrorsStatusExceptions#noTranslation(String, String)
     */
    public static StatusRuntimeException noTranslation(@NotNull String source) {
        return noTranslation(source, "AUTO");
    }

    /**
     * This error should be thrown if no translation found.
     * <b>Note:</b> this error should be used only if translation failed due to absence in
     * supported translation providers.
     * @param source the text fail to be translated
     * @param direction translation direction
     */
    public static StatusRuntimeException noTranslation(@NotNull String source, @NotNull String direction) {
        return Status.NOT_FOUND.augmentDescription(
                format("Can not translate '%s' in translation direction '%s'", source, direction)
        ).asRuntimeException();
    }

    /**
     * This error should be thrown whenever gotten authorization token is invalid or expired.
     */
    public static StatusRuntimeException expiredAuthToken() {
        return Status.UNAUTHENTICATED.augmentDescription(
                "Specified token is expired, call 'register' or 'authorize' rpc to get new valid token."
        ).asRuntimeException();
    }

    /**
     * This error should be thrown whenever failed to  register new user.
     */
    public static StatusRuntimeException registrationFailure() {
        return Status.UNAUTHENTICATED.augmentDescription(
                "Failed to register new user, most likely due to login is already used."
        ).asRuntimeException();
    }

    public static StatusRuntimeException authorizationFailure() {
        return Status.UNAUTHENTICATED.augmentDescription(
                "Failed to authorize user, most likely due to incorrect credentials."
        ).asRuntimeException();
    }
}
