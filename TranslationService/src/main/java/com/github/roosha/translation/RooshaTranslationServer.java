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

import com.github.roosha.proto.translation.AuthorizationInterceptor;
import com.sun.xml.internal.ws.api.client.ServiceInterceptor;
import com.sun.xml.internal.ws.api.client.ServiceInterceptorFactory;
import io.grpc.BindableService;
import io.grpc.Server;
import io.grpc.ServerBuilder;
import io.grpc.ServerInterceptors;
import io.grpc.netty.GrpcSslContexts;
import io.grpc.netty.NettyServerBuilder;
import io.netty.handler.ssl.SslContext;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import javax.annotation.PreDestroy;
import javax.net.ssl.SSLContext;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Scanner;

@Component
public class RooshaTranslationServer implements TranslationServer {
    private Integer port = 1543;

    @Autowired
    private BindableService service;

    private Server server;

    @Override
    public void start() throws IOException {
        final File privateKeyFile = new File(getClass().getResource("/security/server.pkcs8.key").getFile());
        final File certicateFile = new File(getClass().getResource("/security/server.crt").getFile());

        final SslContext sslContext = GrpcSslContexts.forServer(certicateFile, privateKeyFile).build();
        server = NettyServerBuilder.forPort(port)
                                   .sslContext(sslContext)
                                   .addService(ServerInterceptors.intercept(service, new AuthorizationInterceptor()))
                                   .build();
        System.out.println(server == null);
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
