package com.github.roosha.server;

import io.grpc.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;

import javax.annotation.PreDestroy;
import java.io.IOException;

import static com.github.roosha.server.util.Assertions.assertNotNull;

/**
 * This server implementation is test purpose only. It's intended relieve process of testing integration with client.
 */
@Component
public class InsecureRooshaServer implements RooshaServer {
    private static final Logger log = LoggerFactory.getLogger(InsecureRooshaServer.class);

    private final int port;
    private final BindableService service;
    private final ServerInterceptor[] interceptors;

    private Server server;

    @Autowired
    public InsecureRooshaServer(
            @Qualifier("serverPort") Integer port,
            ServerInterceptor[] interceptors,
            BindableService service) {
        assertNotNull(port, "Port should be not null");
        assertNotNull(interceptors, "Interceptors should be not null");
        assertNotNull(service, "Service should be not null");

        this.port = port;
        this.interceptors = interceptors;
        this.service = service;
    }

    @Override
    public void start() throws IOException {
        log.info("Start insecure server on port {}.", port);

        final ServerServiceDefinition serviceWithInterceptors = ServerInterceptors.intercept(service, interceptors);
        server = ServerBuilder.forPort(port).addService(serviceWithInterceptors).build();
        server.start();
    }

    @Override
    @PreDestroy
    public void stop() {
        log.info("Shutdown insecure server.");
        server.shutdown();
    }

    @Override
    public void blockAndWait() throws InterruptedException {
        server.awaitTermination();
    }
}
