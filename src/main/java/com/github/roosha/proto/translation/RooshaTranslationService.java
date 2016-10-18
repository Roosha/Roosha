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

package com.github.roosha.proto.translation;

import com.github.roosha.proto.commons.CommonsProto.Void;
import com.github.roosha.proto.translation.RooshaTranslationServiceGrpc.RooshaTranslationServiceImplBase;
import com.github.roosha.proto.translation.TranslationServiceProto.TranslationRequest;
import com.github.roosha.proto.translation.TranslationServiceProto.Translations;
import com.github.roosha.translation.providers.RawTranslation;
import com.github.roosha.translation.providers.TranslationProvider;
import io.grpc.stub.StreamObserver;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import static com.github.roosha.proto.translation.Errors.noTranslation;

@Component
public class RooshaTranslationService extends RooshaTranslationServiceImplBase {
    @Autowired
    private TranslationProvider provider;

    @Override
    public void translate(TranslationRequest request, StreamObserver<Translations> responseObserver) {
        try {
            final Translations.Builder responseBuilder = Translations.newBuilder();
            final RawTranslation rawTranslation = provider.translate(request.getSource());
            if (rawTranslation != null) {
                rawTranslation.addToProtoTranslationsBuilder(responseBuilder);
                responseBuilder.setSource(request.getSource());
            }
            else {
                throw new Exception("no translation");
            }
            responseObserver.onNext(responseBuilder.build());
            responseObserver.onCompleted();
        } catch (Throwable t) {
            responseObserver.onError(noTranslation(request.getSource()));
        }
    }

    @Override
    public void proposeUserTranslations(Translations request, StreamObserver<Void> responseObserver) {
        responseObserver.onNext(Void.getDefaultInstance());
        responseObserver.onCompleted();
    }
}
