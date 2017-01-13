package com.github.roosha.server.db;

import lombok.val;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Primary;
import org.springframework.stereotype.Component;
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;

import java.util.ArrayList;
import java.util.List;

import static com.github.roosha.proto.ChangesProto.Change;

@Component
@Primary
public class RedisWorldStorage implements WorldStorage {
    private final JedisPool pool;

    @Autowired
    public RedisWorldStorage(@Qualifier("worldStoragePool") JedisPool pool) {
        this.pool = pool;
    }

    @Override
    public @Nullable List<Change> getWorld(long id) {
        try (val jedis = pool.getResource();
                val lock = new WorldLock(jedis, id)) {
            val rawWorld = jedis.lrange(worldKey(id).getBytes(), 0, -1);
            val result = new ArrayList<Change>(rawWorld.size());
            for (byte[] rawChange : rawWorld) {
                result.add(Change.parseFrom(rawChange));
            }
            return result;
        } catch (Throwable t) {
            t.printStackTrace();
            return null;
        }
    }

    @Override
    public void setWorld(long id, @Nullable List<Change> world) {
        try (val jedis = pool.getResource();
                val lock = new WorldLock(jedis, id)) {
            val key = worldKey(id).getBytes();
            jedis.del(key);
            if (world != null) {
                world.forEach(change -> jedis.rpush(key, change.toByteArray()));
            }
        } catch(Throwable t) {
            t.printStackTrace();
        }
    }

    private static @NotNull String worldKey(long id) {
        return "world-" + id + ":";
    }

    private static @NotNull String lockKey(long id) {
        return "lock-" + id + ":";
    }

    private static class WorldLock implements AutoCloseable {
        private final Jedis jedis;
        private final String key;

        WorldLock(Jedis jedis, long id) {
            this.jedis = jedis;
            this.key = lockKey(id);

            while (jedis.setnx(key, "locked") == 0);
        }

        @Override
        public void close() {
            jedis.del(key);
        }
    }
}
