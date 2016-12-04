package com.github.roosha.server.auth;

import com.github.roosha.proto.CommonsProto.Credentials;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import org.springframework.stereotype.Component;

import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

@Component
public class InMemoryHashMapAuthManager implements AuthManager {
    private long firstFreeId = 1;
    private final ConcurrentMap<String, Long> authorizedUsers = new ConcurrentHashMap<>();
    private final ConcurrentMap<CaseSensitiveString, IdAndPasswordHash> registeredUsers = new ConcurrentHashMap<>();

    @Override
    public long getUserIdByToken(@NotNull String token) {
        return authorizedUsers.getOrDefault(token, -1L);
    }

    @Override
    public synchronized @Nullable String authorize(@NotNull Credentials creds) {
        final IdAndPasswordHash userIdAndPasswordHash =
                registeredUsers.getOrDefault(new CaseSensitiveString(creds.getLogin()), null);
        if (userIdAndPasswordHash != null && creds.getPasswordHash().equals(userIdAndPasswordHash.passwordHash)) {
            final Long userId = userIdAndPasswordHash.id;
            String token;
            do {
                token = UUID.randomUUID().toString();
            } while (authorizedUsers.putIfAbsent(token, userId) != null);
            return token;
        } else {
            return null;
        }
    }

    @Override
    public synchronized @Nullable String register(@NotNull Credentials creds) {
        final CaseSensitiveString caseSensitiveLogin = new CaseSensitiveString(creds.getLogin());
        if (registeredUsers.containsKey(caseSensitiveLogin)) {
            return null;
        }
        final long userId = firstFreeId++;
        registeredUsers.put(caseSensitiveLogin, new IdAndPasswordHash(userId, creds.getPasswordHash()));
        String token;
        do {
            token = UUID.randomUUID().toString();
        } while (authorizedUsers.putIfAbsent(token, userId) != null);
        return token;

    }

    private static class CaseSensitiveString {
        private final String string;

        CaseSensitiveString(String string) {
            this.string = string;
        }

        @Override
        public boolean equals(Object obj) {
            return obj instanceof CaseSensitiveString && string.equalsIgnoreCase(((CaseSensitiveString) obj).string);
        }

        @Override
        public int hashCode() {
            // TODO: afaik, this is not correct because of some strange symbols like ß
            // who convert to combination of different letters when chenging case
            return string.toLowerCase().hashCode();
        }
    }

    private static class IdAndPasswordHash {
        final Long id;
        final String passwordHash;

        IdAndPasswordHash(Long id, String passwordHash) {
            this.id = id;
            this.passwordHash = passwordHash;
        }

        @Override
        public boolean equals(Object obj) {
            return obj instanceof IdAndPasswordHash
                    && id.equals(((IdAndPasswordHash) obj).id)
                    && passwordHash.equals(((IdAndPasswordHash) obj).passwordHash);
        }

        @Override
        public String toString() {
            return "id: " + id + "\npasswordHash: " + passwordHash;
        }
    }
}
