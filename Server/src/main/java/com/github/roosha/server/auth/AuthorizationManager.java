package com.github.roosha.server.auth;

import com.github.roosha.proto.commons.CommonsProto.Credentials;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

public interface AuthorizationManager {
    /**
     * Try to get id of user authorized with this token.
     * @param token user authorization login.
     * @return authorized user id, if token is valid, -1 otherwise.
     */
    long getUserIdByToken(@NotNull String token);

    /**
     * Try to get id of user authorized with this token.
     * @param login user login
     * @return authorized user id, if login is registered, -1 otherwise.
     */
    long getUserIdByLogin(@NotNull String login);

    /**
     * Generate token for user with specified credentials.
     * @param userCredentials authorization credentials
     * @return generated token or {@code null} if authorization failed.
     */
    @Nullable String authorize(@NotNull Credentials userCredentials);

    /**
     * Register and authorize user with specified credentials.
     * @param userCredentials authorization credentials
     * @return generated token if registration dsucceeded, {@code null} otherwise
     */
    @Nullable String register(@NotNull Credentials userCredentials);
}
