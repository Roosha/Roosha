package com.github.roosha.server.db;

import com.github.roosha.proto.ChangesProto.Change;
import lombok.val;
import org.jetbrains.annotations.Nullable;

import java.util.List;

public interface WorldStorage {
    @Nullable List<Change> getWorld(long id);
    default long getHistoryLen(long id) {
        return getWorld(id).size();
    }

    void setWorld(long id, @Nullable List<Change> world);

    default void appendToWorld(long id, @Nullable List<Change> world) {
        List<Change> oldWorld = this.getWorld(id);
        if (oldWorld == null) {
            setWorld(id, world);
        } else if (world == null) {
            setWorld(id, oldWorld);
        } else {
            oldWorld.addAll(world);
            setWorld(id, oldWorld);
        }
    }
}
