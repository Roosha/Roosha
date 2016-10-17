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

package com.github.roosha.translation.providers.yandexdict;

import com.github.roosha.translation.providers.RawTranslation;
import com.github.roosha.translation.providers.TranslationProvider;
import org.jetbrains.annotations.Nullable;
import org.springframework.stereotype.Component;

import java.io.InputStream;
import java.net.URL;
import java.util.Scanner;

import static java.lang.String.format;

@Component
public class YandexDictionaryProvider implements TranslationProvider {
    // TODO: inject API key in runtime
    private final String API_KEY =
            "dict.1.1.20160905T084639Z.02f5a4d75c0b1167.84b38402a3426374a791c5df3e5416e0ef7d9b5c";
    private final String LOOKUP_FORMAT_STRING =
            "https://dictionary.yandex.net/api/v1/dicservice.json/lookup?key=%s&text=%s&lang=%s";
    private final String DEFAULT_LANG = "en-ru";

    @Override
    public @Nullable RawTranslation translate(String source) throws Exception {
        final String query = createQueryString(source);
        final InputStream responseStream = new URL(query).openConnection().getInputStream();
        final Scanner responseScanner = new Scanner(responseStream).useDelimiter("\\A");
        final String response = responseScanner.hasNext() ? responseScanner.next() : "";
        return YDRawTranslation.fromJson(response);
    }

    private String createQueryString(String source) {
        return format(LOOKUP_FORMAT_STRING, API_KEY, source, DEFAULT_LANG);
    }
}
