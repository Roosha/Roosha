package com.github.roosha.server.auth;

import com.github.roosha.proto.commons.CommonsProto.Credentials;
import com.google.protobuf.ByteString;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import org.springframework.stereotype.Component;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;

import static com.github.roosha.server.util.UuidUtil.generateUuidASByteString;

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
    public synchronized @Nullable ByteString authorize(@NotNull Credentials creds) {
        final IdAndPasswordHash userIdAndPasswordHash =
                registeredUsers.getOrDefault(new CaseSensitiveString(creds.getLogin()), null);
        if (userIdAndPasswordHash != null && creds.getPasswordHash().equals(userIdAndPasswordHash.passwordHash)) {
            final Long userId = userIdAndPasswordHash.id;
            ByteString token;
            do {
                token = generateUuidASByteString();
            } while (authorizedUsers.putIfAbsent(token, userId) != null);
            return token;
        } else {
            return null;
        }
    }

    @Override
    public synchronized @Nullable ByteString register(@NotNull Credentials creds) {
        final CaseSensitiveString caseSensitiveLogin = new CaseSensitiveString(creds.getLogin());
        if (registeredUsers.containsKey(caseSensitiveLogin)) {
            return null;
        }
        final long userId = firstFreeId++;
        registeredUsers.put(caseSensitiveLogin, new IdAndPasswordHash(userId, creds.getPasswordHash()));
        ByteString token;
        do {
            token = generateUuidASByteString();
        } while (authorizedUsers.putIfAbsent(token, userId) != null);
        return token;

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
            // TODO: afaik, this is not correct because of some strange symbols like ß
            // who convert to combination of different letters when chenging case
            return string.toLowerCase().hashCode();
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

        @Override
        public String toString() {
            return "id: " + id + "\npasswordHash: " + passwordHash;
        }
    }
}
