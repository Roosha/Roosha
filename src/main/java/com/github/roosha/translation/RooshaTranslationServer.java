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

import io.grpc.BindableService;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import javax.annotation.PreDestroy;
import java.io.IOException;

@Component
public class RooshaTranslationServer implements TranslationServer {
    private Integer port = 1543;

    @Autowired
    private BindableService service;

    private Server server;

    @Override
    public void start() throws IOException {
        server = ServerBuilder.forPort(port).addService(service).build();
        server.start();
    }

    @Override
    public void blockAndWait() throws InterruptedException {
        server.awaitTermination();
    }

    @Override
    @PreDestroy
    public void stop() {
        server.shutdown();
    }
}
