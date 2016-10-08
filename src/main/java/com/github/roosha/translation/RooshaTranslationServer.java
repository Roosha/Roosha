/*
 *  Copyright (c) 2016 Leo Khotov
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  version 3 along with this work.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

package com.github.roosha.translation;

import com.github.roosha.translation.proto.RooshaTranslationService;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;

public class RooshaTranslationServer {
    private int port;
    private @Nullable Server server;

    public RooshaTranslationServer(int port) {
        this.port = port;
        this.server = ServerBuilder.forPort(this.port).addService(new RooshaTranslationService()).build();
    }

    public void start() throws IOException {
        if (server != null) {
            server.start();
        }
        Runtime.getRuntime().addShutdownHook(new Thread(this::stop));
    }

    public void stop() {
        if (server != null) {
            server.shutdown();
        }
    }

    private void blockUntilShutdown() throws InterruptedException {
        if (server != null) {
            server.awaitTermination();
        }
    }

    public static void main(String[] args) throws Exception {
        final RooshaTranslationServer translationServer = new RooshaTranslationServer(1488);
        translationServer.start();
        translationServer.blockUntilShutdown();
    }
}
