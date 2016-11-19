package com.github.roosha.server;

import io.grpc.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import javax.annotation.PreDestroy;
import java.io.IOException;

/**
 * This server implementation is test purpose only. It's intended relieve process of testing integration with client.
 */
@Component
public class InsecureRooshaServer implements RooshaServer {
    private final int port = 1543;

    private final BindableService service;

    private final ServerInterceptor[] interceptors;

    private Server server;

    @Autowired
    public InsecureRooshaServer(
            ServerInterceptor[] interceptors,
            BindableService service) {
        this.interceptors = interceptors;
        this.service = service;
    }

    @Override
    public void start() throws IOException {
        final ServerServiceDefinition serviceWithInterceptors = ServerInterceptors.intercept(service, interceptors);
        server = ServerBuilder.forPort(port).addService(serviceWithInterceptors).build();
        server.start();
    }

    @Override
    @PreDestroy
    public void stop() {
        server.shutdown();
    }

    @Override
    public void blockAndWait() throws InterruptedException {
        server.awaitTermination();
    }
}
