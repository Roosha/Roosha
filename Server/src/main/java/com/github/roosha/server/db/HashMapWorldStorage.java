package com.github.roosha.server.db;

import com.github.roosha.proto.ChangesProto.Change;
import org.jetbrains.annotations.Nullable;
import org.springframework.stereotype.Component;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Component
public class HashMapWorldStorage implements WorldStorage {
    private final Map<Long, List<Change>> worlds = new HashMap<>();

    @Override
    public synchronized @Nullable List<Change> getWorld(long id) {
        return worlds.getOrDefault(id, null);
    }

    @Override
    public synchronized void setWorld(long id, @Nullable List<Change> world) {
        worlds.put(id, world);
    }
}
