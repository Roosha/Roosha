package com.github.roosha.server.util;

import org.jetbrains.annotations.Nullable;

public class Assertions {
    public static void assertNotNull(@Nullable Object obj) {
        assertNotNull(obj, null);
    }

    public static void assertNotNull(@Nullable Object obj, @Nullable String message) {
        if (obj == null) {
            if (message == null) {
                throw new NullPointerException();
            } else {
                throw new NullPointerException(message);
            }
        }
    }

}
