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
import com.github.roosha.proto.translation.TranslationServiceProto.Translation;
import com.github.roosha.proto.translation.TranslationServiceProto.TranslationRequest;
import com.github.roosha.proto.translation.TranslationServiceProto.Translations;
import io.grpc.ManagedChannel;
import io.grpc.ManagedChannelBuilder;
import org.jetbrains.annotations.NotNull;

import static java.lang.String.join;

public class RooshaTranslationServerTest {
    private final RooshaTranslationServiceGrpc.RooshaTranslationServiceBlockingStub blockingStub;
    private final RooshaTranslationServiceGrpc.RooshaTranslationServiceStub asynchronousStub;
    private final ManagedChannel channel;


    public RooshaTranslationServerTest(String host, int port) {
        this.channel = ManagedChannelBuilder.forAddress(host, port).usePlaintext(true).build();
        blockingStub = RooshaTranslationServiceGrpc.newBlockingStub(channel);
        asynchronousStub = RooshaTranslationServiceGrpc.newStub(channel);
    }

    public static void main(String[] args) {
        final RooshaTranslationServerTest client = new RooshaTranslationServerTest("127.0.0.1", 1543);

        for (String str : new String[]{"exhibit", "time"}) {
            @NotNull Translations translations = Translations.getDefaultInstance();
            try {
                translations = client.translate(str);
            } catch (Exception e) {
                System.err.println("Error occurred");
                //                e.printStackTrace();
            }

            System.out.println("source: " + translations.getSource());

            for (Translation translation : translations.getTranslationList()) {
                System.out.println("Translation:");
                final String targets = join(", ", translation.getTargetList());
                final String examples = join(", ", translation.getExampleList());
                System.out.println("targets: " + targets);
                System.out.println("examples: " + examples);
                System.out.println();
            }
            System.out.println("---------------------------");
        }
    }

    private Translations translate(String source) {
        final TranslationRequest request = TranslationRequest.newBuilder().setSource(source).build();
        return blockingStub.translate(request);
    }
}
