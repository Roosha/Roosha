package com.github.roosha.server.auth;

import com.github.roosha.proto.ChangesProto;
import com.github.roosha.proto.CommonsProto.Credentials;
import lombok.val;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Primary;
import org.springframework.stereotype.Component;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;

import java.util.ArrayList;
import java.util.UUID;

import static com.github.roosha.server.util.Assertions.assertNotNull;

@Component
@Primary
public class RedisAuthManager implements AuthManager {
    private static final Logger log = LoggerFactory.getLogger(AuthManager.class);

    private static final String LAST_USED_ID = "lastUsedId";
    private static final int TOKEN_EXPIRATION_SECONDS = 30 * 24 * 60 * 60;

    private final JedisPool authPool;

    @Autowired
    public RedisAuthManager(@Qualifier("authRedisPool") JedisPool authPool) {
        assertNotNull(authPool);
        this.authPool = authPool;

        try (val jedis = authPool.getResource()) {
            jedis.setnx(LAST_USED_ID, "0");
        } catch (Throwable t) {
            log.warn("Exception while initializing last user field.", t);
        }
    }

    @Override
    public long getUserIdByToken(@NotNull String token) {
        try (val jedis = authPool.getResource()) {
            val value = jedis.get(tokenToIdKey(token));
            return value == null ? -1 : Long.parseLong(value);
        } catch (Throwable t) {
            log.warn("Exception in attempt to extract user id by token.", t);
            return -1;
        }
    }

    @Override
    public @Nullable String authorize(@NotNull Credentials userCredentials) { // TODO: add machine_id
        try (val jedis = authPool.getResource()) {
            val idValue = jedis.get(loginToIdKey(userCredentials.getLogin()));
            if (idValue == null) {
                return null;
            }
            val id = Long.parseLong(idValue);
            val passwordHash = jedis.get(idToPasswordKey(id));
            return passwordHash == null || !passwordHash.equals(userCredentials.getPasswordHash()) ?
                    null : generateToken(jedis, id);
        } catch (Throwable t) {
            log.warn("Exception while checking user credentials.", t);
            return null;
        }
    }

    @Override
    public @Nullable String register(@NotNull Credentials userCredentials) { // TODO: add machine_id
        try (val jedis = authPool.getResource()) {
            val loginKey = loginToIdKey(userCredentials.getLogin());
            if (jedis.setnx(loginKey, "tmp") == 0) {
                return null;
            }
            val id = jedis.incr(LAST_USED_ID);
            jedis.set(loginKey, id.toString());
            jedis.set(idToPasswordKey(id), userCredentials.getPasswordHash());
            return generateToken(jedis, id);
        } catch (Throwable t) {
            log.warn("Exception while registering user.", t);
            return null;
        }
    }

    @Override
    public @Nullable Integer getMachineId(@NotNull Credentials userCredentials) {
        try (val jedis = authPool.getResource()) {
            val loginKey = loginToMachineIdKey(userCredentials.getLogin());
            val userMacs = jedis.lrange(loginKey.getBytes(), 0, -1);
            if(userMacs == null || userMacs.size() == 0) {
                jedis.rpush(loginKey.getBytes(), userCredentials.getMacAddress().getBytes());
                return 1;
            }
            val result = new ArrayList<String>(userMacs.size());
            for(byte[] rawString : userMacs) {
                result.add(new String(rawString));
            }
            int index = result.indexOf(userCredentials.getMacAddress());
            if(index == -1) {
                jedis.rpush(loginKey.getBytes(), userCredentials.getMacAddress().getBytes());
                return result.size() + 1;
            }
            return index + 1;

        } catch (Throwable t) {
            log.warn("Exception while getting machine id of user.", t);
            return null;
        }
    }

    private @NotNull String generateToken(Jedis jedis, long id) {
        for (int i = 0; i < 100; i++) {
            val newToken = UUID.randomUUID().toString();
            val key = tokenToIdKey(newToken);
            if (jedis.setnx(key, Long.toString(id)) == 1) {
                jedis.expire(key, TOKEN_EXPIRATION_SECONDS);
                return newToken;
            }
        }
        throw new RuntimeException("failed to generate unique token for 100 times");
    }

    private @NotNull String tokenToIdKey(@NotNull String token) {
        return "token:" + token;
    }

    /**
     * Note, that in database login is stored in lower case, so it is case-insensitive.
     * @param login user login. it is not required to convert login to lower case before passing to this function.
     */
    private @NotNull String loginToIdKey(@NotNull String login) {
        return "lclogin:" + login.toLowerCase();
    }

    private @NotNull String loginToMachineIdKey(@NotNull String login) {
        return "machinelogin:" + login.toLowerCase();
    }

    private @NotNull String idToPasswordKey(long id) {
        return "pass:" + id;
    }
}
