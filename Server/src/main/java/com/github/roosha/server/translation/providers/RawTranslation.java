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

package com.github.roosha.server.translation.providers;

import com.github.roosha.proto.TranslationServiceProto.Translations;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

/**
 * External service-specific translation format.
 */
public interface RawTranslation {
    /**
     * Convert raw translation to Roosha protocol format.
     * Builder used as the return type to ease composition of several raw translations(from different translation
     * services).
     * <b>Note:</b> this method sets {@link Translations#source_}.
     * @return this translation in protobuf builder representation
     */
    @Nullable Translations.Builder convertToProtoTranslationsBuilder();

    /**
     * Add this raw translation to {@code target}.
     * <b>Note:</b> this method doesn't set {@link Translations#source_}. Furthermore, it doesn't check whether
     * {@code target.source_} corresponds to this translation source or not.
     * @param target translations builder to be supplemented.
     */
    void addToProtoTranslationsBuilder(@NotNull Translations.Builder target);
}
