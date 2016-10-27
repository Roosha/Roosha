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

import com.github.roosha.proto.translation.RooshaTranslationServiceGrpc;
import com.github.roosha.proto.translation.RooshaTranslationServiceGrpc.RooshaTranslationServiceBlockingStub;
import com.github.roosha.proto.translation.RooshaTranslationServiceGrpc.RooshaTranslationServiceStub;
import com.github.roosha.proto.translation.TranslationServiceProto.TranslationRequest;
import com.github.roosha.proto.translation.TranslationServiceProto.Translations;
import com.github.roosha.translation.config.TranslationServiceConfig;
import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import io.grpc.netty.GrpcSslContexts;
import io.grpc.netty.NettyChannelBuilder;
import io.netty.handler.ssl.util.InsecureTrustManagerFactory;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import java.io.File;
import java.io.IOException;

import static org.junit.Assert.assertEquals;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = TranslationServiceConfig.class)
public class RooshaTranslationServerTest {
    private RooshaTranslationServiceBlockingStub blockingStub;
    private RooshaTranslationServiceStub asynchronousStub;
    private ManagedChannel channel;

    @Autowired
    private TranslationServer server;

    @Before
    public void setUp() throws IOException {
        final String host = "127.0.0.1";
        final int port = 1543;


        this.channel = NettyChannelBuilder.forAddress(host, port)
                                          .sslContext(GrpcSslContexts.forClient()
//                                                                     .trustManager(getClass().getResourceAsStream("/security/ca.crt"))
// Now, in test cases, we're using self signed SSL sertificate on the Server, thereby we disable sertificate validation.
                                                                     .trustManager(InsecureTrustManagerFactory.INSTANCE)
                                                                     .build())
                                          .build();
        blockingStub = RooshaTranslationServiceGrpc.newBlockingStub(channel);
        asynchronousStub = RooshaTranslationServiceGrpc.newStub(channel);

        server.start();
    }

    @After
    public void shutDown() {
        server.stop();
    }

    @Test
    public void translationServiceRespectsSource() {
        for (String source : new String[]{"source", "exhibit", "house", "noise"}) {
            final Translations response = translate(source);
            System.out.println(response);
            assertEquals(source, response.getSource());
        }
    }

    private Translations translate(String source) {
        final TranslationRequest request = TranslationRequest.newBuilder().setSource(source).build();
        return blockingStub.translate(request);
    }
}
