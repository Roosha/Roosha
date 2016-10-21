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

package com.github.roosha.proto.translation;

import io.grpc.Status;
import io.grpc.StatusException;
import org.jetbrains.annotations.NotNull;

import static java.lang.String.format;

/**
 * This class provides default error statuses for translation service protocol.
 */
public class Errors {
    /**
     * Translation failure status exception for default translation direction.
     * @param source
     * @return
     * @see Errors#noTranslation(String, String)
     */
    public static StatusException noTranslation(@NotNull String source) {
        return noTranslation(source, "AUTO");
    }

    /**
     * This error should be thrown if no translation found.
     * <b>Note:</b> this error should be used only if translation failed due to absence in
     * supported translation providers.
     * @param source the text fail to be translated
     * @param direction translation direction
     * @return
     */
    public static StatusException noTranslation(@NotNull String source, @NotNull String direction) {
        return Status.NOT_FOUND.augmentDescription(
                format("Can not translate '%s' in translation direction '%s'", source, direction)
        ).asException();
    }
}
