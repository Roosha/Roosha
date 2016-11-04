package com.github.roosha.server;

import io.grpc.BindableService;
import io.grpc.Server;
import io.grpc.ServerBuilder;
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

    @Autowired
    private BindableService service;

    private Server server;

    @Override
    public void start() throws IOException {
        server = ServerBuilder.forPort(port).addService(service).build();
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
