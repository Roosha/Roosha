package com.github.roosha.server.auth;

import com.github.roosha.proto.commons.CommonsProto.Credentials;
import com.google.protobuf.ByteString;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import org.springframework.stereotype.Component;

import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

@Component
public class InMemoryHashMapAuthorizationManager implements AuthorizationManager {
    private long firstFreeId = 1;
    private final ConcurrentMap<ByteString, Long> authorizedUsers = new ConcurrentHashMap<>();
    private final ConcurrentMap<CaseSensitiveString, IdAndPasswordHash> registeredUsers = new ConcurrentHashMap<>();

    @Override
    public long getUserIdByToken(@NotNull ByteString token) {
        return authorizedUsers.getOrDefault(token, -1L);
    }

    @Override
    public long getUserIdByLogin(@NotNull String login) {
        final IdAndPasswordHash idAndPasswordHash = registeredUsers.getOrDefault(new CaseSensitiveString(login), null);
        return idAndPasswordHash != null ? idAndPasswordHash.id : -1L;
    }

    @Override
    public synchronized @Nullable ByteString authorize(@NotNull Credentials userCredentials) {
        final IdAndPasswordHash userIdAndPasswordHash =
                registeredUsers.getOrDefault(new CaseSensitiveString(userCredentials.getLogin()), null);
        if (userIdAndPasswordHash != null) {
            final Long userId = userIdAndPasswordHash.id;
            ByteString token;
            do {
                token = generateUuidASByteString();
            } while (authorizedUsers.putIfAbsent(token, userId).equals(userId));
            return token;
        } else {
            return null;
        }
    }

    @Override
    public synchronized @Nullable ByteString register(@NotNull Credentials userCredentials) {
        final CaseSensitiveString caseSensitiveLogin = new CaseSensitiveString(userCredentials.getLogin());
        if (registeredUsers.containsKey(caseSensitiveLogin)) {
            return null;
        }
        final long userId = firstFreeId++;
        registeredUsers.put(caseSensitiveLogin, new IdAndPasswordHash(userId, userCredentials.getPasswordHash()));
        ByteString token;
        do {
            token = generateUuidASByteString();
        } while (authorizedUsers.putIfAbsent(token, userId).equals(userId));
        return token;

    }

    private @NotNull ByteString generateUuidASByteString() {
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

    private static class CaseSensitiveString {
        private final String string;

        public CaseSensitiveString(String string) {
            this.string = string;
        }

        @Override
        public boolean equals(Object obj) {
            return obj instanceof CaseSensitiveString && string.equalsIgnoreCase(((CaseSensitiveString) obj).string);
        }

        @Override
        public int hashCode() {
            return string.hashCode();
        }
    }

    private static class IdAndPasswordHash {
        final Long id;
        final ByteString passwordHash;

        public IdAndPasswordHash(Long id, ByteString passwordHash) {
            this.id = id;
            this.passwordHash = passwordHash;
        }

        @Override
        public boolean equals(Object obj) {
            return obj instanceof IdAndPasswordHash
                    && id.equals(((IdAndPasswordHash) obj).id)
                    && passwordHash.equals(((IdAndPasswordHash) obj).passwordHash);
        }
    }
}
