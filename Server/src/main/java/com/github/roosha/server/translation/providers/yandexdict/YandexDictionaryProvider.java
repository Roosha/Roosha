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

import com.github.roosha.server.translation.providers.RawTranslation;
import com.github.roosha.server.translation.providers.TranslationProvider;
import org.jetbrains.annotations.NotNull;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.net.URL;
import java.net.URLEncoder;
import java.util.Scanner;

import static com.github.roosha.server.util.Assertions.assertNotNull;
import static java.lang.String.format;

@Component
public class YandexDictionaryProvider implements TranslationProvider {
    private final String API_KEY;

    private final String LOOKUP_FORMAT_STRING =
            "https://dictionary.yandex.net/api/v1/dicservice.json/lookup?key=%s&text=%s&lang=%s";
    private final String DEFAULT_LANG = "en-ru";

    @Autowired
    public YandexDictionaryProvider(@Qualifier("yandexDictionaryApiKey") String API_KEY) {
        assertNotNull(API_KEY, "api key must not be null");

        this.API_KEY = API_KEY;
    }

    @Override
    public @NotNull RawTranslation translate(@NotNull String source) {
        try {
            final String query = createQueryString(source);
            final InputStream responseStream = new URL(query).openConnection().getInputStream();
            final Scanner responseScanner = new Scanner(responseStream).useDelimiter("\\A");
            final String response = responseScanner.hasNext() ? responseScanner.next() : "";
            return YDRawTranslation.fromJson(response);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private String createQueryString(String source, String... lang) {
        try {
            source = URLEncoder.encode(source, "UTF-8");
            final String languge = lang.length == 0 ? DEFAULT_LANG : lang[0];
            return format(LOOKUP_FORMAT_STRING, API_KEY, source, languge);
        } catch (UnsupportedEncodingException e) {
            throw new RuntimeException(e);
        }
    }
}
