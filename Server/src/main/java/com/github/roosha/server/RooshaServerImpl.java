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

package com.github.roosha.server;

import io.grpc.*;
import io.grpc.netty.GrpcSslContexts;
import io.grpc.netty.NettyServerBuilder;
import io.netty.handler.ssl.SslContext;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Primary;
import org.springframework.stereotype.Component;

import javax.annotation.PreDestroy;
import java.io.File;
import java.io.IOException;

import static com.github.roosha.server.util.Assertions.assertNotNull;

@Component
@Primary
public class RooshaServerImpl implements RooshaServer {
    private final int port;
    private final ServerServiceDefinition serviceDefinition;
    private Server server;

    @Autowired
    private RooshaServerImpl(
            @Qualifier("serverPort") Integer port,
            BindableService service,
            ServerInterceptor[] interceptors) {
        assertNotNull(port, "port must not be not null");
        assertNotNull(service, "service must be not null");
        assertNotNull(interceptors, "interceptors must be not null");

        this.port = port;
        serviceDefinition = ServerInterceptors.intercept(service, interceptors);
    }

    @Override
    public void start() throws IOException {
        // TODO: get certificate files via dependency injection
        final File privateKeyFile = new File(getClass().getResource("/security/server.pkcs8.key").getFile());
        final File certFile = new File(getClass().getResource("/security/server.crt").getFile());

        final SslContext sslContext = GrpcSslContexts.forServer(certFile, privateKeyFile).build();
        server = NettyServerBuilder.forPort(port)
                                   .sslContext(sslContext)
                                   .addService(serviceDefinition)
                                   .build();
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
