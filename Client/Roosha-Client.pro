TEMPLATE = app

QT += qml quick widgets core quickwidgets quickcontrols2
CONFIG += c++11

QMAKE_CXX = g++

# grpc
QMAKE_LIBS += -L/usr/local/lib -lgrpc++ -lgrpc++_reflection -lprotobuf -lpthread -ldl

SOURCES += main.cpp \
    Core/centralcontroller.cpp \
    Core/hotkeylistener.cpp \
    Core/card.cpp \
    Core/dbcard.cpp \
    GUI/guimanager.cpp \
    Helpers/bootstrap.cpp \
    Helpers/configuremanager.cpp \
    DB/dbmanager.cpp \
    Network/networkmanager.cpp \
    Core/applicationdaemon.cpp \
    Core/translation.cpp \
    GUI/translationcontroller.cpp \
    Helpers/qmlconvertation.cpp \
    Network/network_manager.cpp \
    Test/Network/translations_test_slot_holder.cpp \
    Core/changes.cpp \
    Core/cardbuilder.cpp \
    Network/roosha_service.cpp \
    Network/server_response.cpp \
    Network/authentication_manager.cpp

RESOURCES += \
    QML/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH = /home/alex/IDE/Qt/5.6/gcc_64/qml

# Default rules for deployment.
include(deployment.pri)
include(lib/qtservice/src/qtservice.pri)
include(lib/QHotkey/qhotkey.pri)

HEADERS += \
    Core/centralcontroller.h \
    Core/hotkeylistener.h \
    Core/card.h \
    Core/dbcard.h \
    GUI/guimanager.h \
    Helpers/bootstrap.h \
    Helpers/configuremanager.h \
    DB/dbmanager.h \
    Network/networkmanager.h \
    Core/applicationdaemon.h \
    Core/translation.h \
    GUI/translationcontroller.h \
    Helpers/qmlconvertation.h \
    Core/ichange.h \
    Core/changes.h \
    Core/cardbuilder.h \
    Network/network_manager.h \
    Test/Network/translations_test_slot_holder.h \
    Network/roosha_service.h \
    Network/server_response.h \
    Network/authentication_manager.h

# protobuf generated files
SOURCES += Network/Proto/commons.pb.cc \
    Network/Proto/roosha_service.pb.cc \
    Network/Proto/roosha_service.grpc.pb.cc

HEADERS += Network/Proto/commons.pb.h \
    Network/Proto/roosha_service.pb.h \
    Network/Proto/roosha_service.grpc.pb.h

OTHER_FILES += Network/Proto/commons.proto \
    Network/Proto/roosha_service.proto
#-------------------------------

DISTFILES += \
    codeStyle.astylerc
