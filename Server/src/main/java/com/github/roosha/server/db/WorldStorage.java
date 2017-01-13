package com.github.roosha.server.db;

import com.github.roosha.proto.ChangesProto.Change;
import org.jetbrains.annotations.Nullable;

import java.util.List;

public interface WorldStorage {
    @Nullable List<Change> getWorld(long id);

    void setWorld(long id, @Nullable List<Change> world);
}
