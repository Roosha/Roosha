package com.github.roosha.server.translation;

import com.github.roosha.proto.TranslationServiceProto.Translations;
import lombok.val;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;
import redis.clients.jedis.JedisPool;

import static com.github.roosha.server.util.Assertions.assertNotNull;

@Component
public class TranslationsCacheManager {
    private static final Logger log = LoggerFactory.getLogger(TranslationsCacheManager.class);

    private final int EXPIRATION_TIME_SECONDS = 63 * 24 * 60 * 60;

    private final JedisPool jedisPool;

    @Autowired
    public TranslationsCacheManager(@Qualifier("translationsCachePool") JedisPool jedisPool) {
        assertNotNull(jedisPool, "translations cache jedis pool should not be null");

        this.jedisPool = jedisPool;
    }

    public @Nullable Translations translate(@NotNull String source) {
        try (val jedis = jedisPool.getResource()) {
            val value = jedis.get(source.getBytes());
            if (value == null) {
                return null;
            }
            return Translations.parseFrom(value);
        } catch (Throwable t) {
            log.warn("Exception while checking out translation from cache", t);
            return null;
        }
    }

    public void save(@NotNull Translations translations) {
        try (val jedis = jedisPool.getResource()) {
            jedis.set(translations.getSource().getBytes(), translations.toByteArray());
            jedis.expire(translations.getSource(), EXPIRATION_TIME_SECONDS);
        } catch (Throwable t) {
            log.warn("Exception while saving translation to cache", t);
        }
    }
}
