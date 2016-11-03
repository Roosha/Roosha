package com.github.roosha.server.util;

import com.google.protobuf.ByteString;
import org.jetbrains.annotations.NotNull;

import java.util.UUID;

public class UuidUtil {
    public static @NotNull ByteString generateUuidASByteString() {
        final UUID uuid = UUID.randomUUID();
        final long hi = uuid.getMostSignificantBits();
        final long lo = uuid.getLeastSignificantBits();
        return ByteString.copyFrom(new byte[] {
                (byte) (hi >>> 56),
                (byte) (hi >>> 48),
                (byte) (hi >>> 40),
                (byte) (hi >>> 32),
                (byte) (hi >>> 16),
                (byte) (hi >>> 8),
                (byte) hi,
                (byte) (lo >>> 56),
                (byte) (lo >>> 48),
                (byte) (lo >>> 40),
                (byte) (lo >>> 32),
                (byte) (lo >>> 16),
                (byte) (lo >>> 8),
                (byte) lo
        });
    }
}

