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

package com.github.roosha.server.translation.providers.yandexdict;

import com.github.roosha.proto.translation.TranslationServiceProto;
import com.github.roosha.proto.translation.TranslationServiceProto.Translation;
import com.github.roosha.proto.translation.TranslationServiceProto.Translations;
import com.github.roosha.server.translation.providers.RawTranslation;
import com.google.gson.Gson;
import com.google.gson.annotations.SerializedName;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.util.Arrays;

import static java.util.Arrays.stream;

@SuppressWarnings("ConstantConditions")
class YDRawTranslation implements RawTranslation {
    @SerializedName("def")
    private @Nullable Definition[] definitions;

    @Override
    public @Nullable Translations.Builder convertToProtoTranslationsBuilder() {
        if (definitions == null || definitions.length == 0 || definitions[0] == null) {
            return null;
        }

        final Translations.Builder resultBuilder = Translations.newBuilder();
        resultBuilder.setSource(definitions[0].text); // definitions always contain the source text in 'text' field.

        addToProtoTranslationsBuilder(resultBuilder);
        return resultBuilder;
    }

    @Override
    public void addToProtoTranslationsBuilder(@NotNull Translations.Builder target) {
        if (definitions == null || definitions.length == 0) {
            return;
        }
        stream(definitions).filter(def -> def != null)
                           .map(def -> def.translations)
                           .filter(trs -> trs != null)
                           .flatMap(Arrays::stream)
                           .filter(tr -> tr != null)
                           .map(YDTranslation::toProtoTranslation)
                           .filter(tr -> tr != null)
                           .forEach(target::addTranslation);
    }

    public static @NotNull RawTranslation fromJson(@NotNull String json) {
        return new Gson().fromJson(json, YDRawTranslation.class);
    }

    private static class BaseFields {
        @SerializedName("text")
        @NotNull String text = "";

        @SerializedName("number")
        @Nullable String number;

        @SerializedName("partOfSpeech")
        @Nullable String partOfSpeech;

        @SerializedName("gender")
        @Nullable String gender;
    }

    private static class Definition extends BaseFields {
        @SerializedName("ts")
        @Nullable String transcription;

        @SerializedName("tr")
        @Nullable YDTranslation[] translations;
    }

    private static class YDTranslation extends BaseFields {
        /**
         * Synonyms in target language.
         */
        @SerializedName("syn")
        @Nullable BaseFields[] targets;

        /**
         * Synonyms in source language.
         */
        @SerializedName("mean")
        @Nullable BaseFields[] synonims;

        /**
         * Examples of usage in source language.
         */
        @SerializedName("ex")
        @Nullable BaseFields[] examples;

        @Nullable Translation toProtoTranslation() {
            final Translation.Builder builder =
                    Translation.newBuilder();
            if (targets != null) {
                stream(targets).filter(tg -> tg != null).map(tg -> tg.text.trim()).filter(s -> s.length() > 0).forEach(builder::addTarget);
            }
            if (examples != null) {
                stream(examples).filter(ex -> ex != null).map(ex -> ex.text).forEach(builder::addExample);
            }
            builder.setProvider(TranslationServiceProto.TranslationProvider.YANDEX_DICTIONARY);
            final Translation result = builder.build();
            if (result.getExampleList().size() + result.getTargetList().size() > 0) {
                return result;
            }
            return null;
        }
    }
}
