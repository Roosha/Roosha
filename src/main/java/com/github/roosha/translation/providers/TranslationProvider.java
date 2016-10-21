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

package com.github.roosha.translation.providers;

import org.jetbrains.annotations.NotNull;

/**
 * Interface for external translation service adapters.
 */
public interface TranslationProvider {
    /**
     * Blocking external translation service call.
     * @param source text to be translated
     * @return result of translation
     * @throws Exception
     */
    @NotNull RawTranslation translate(@NotNull String source) throws Exception;
}
