TEMPLATE = app

QT += qml quick widgets core quickwidgets quickcontrols2 concurrent
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
    Core/applicationdaemon.cpp \
    Core/translation.cpp \
    GUI/translationcontroller.cpp \
    Helpers/qmlconvertation.cpp \
    Test/Network/translations_test_slot_holder.cpp \
    Core/changes.cpp \
    Network/server_response.cpp \
    Network/authentication_manager.cpp \
    Network/roosha_service_connector.cpp \
    Network/network_manager.cpp \
    Helpers/protobuf_converter.cpp \
    Core/world.cpp \
    Test/Core/changes/worldtest.cpp \
    GUI/system_tray.cpp \
    Test/Core/changes/worldtest.cpp \
    GUI/cardcreationcontroller.cpp \
    GUI/cardlistcontroller.cpp \
    GUI/authentication_controller.cpp

RESOURCES += \
    QML/qml.qrc \
    Img/img.qrc

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
    Core/applicationdaemon.h \
    Core/translation.h \
    GUI/translationcontroller.h \
    Helpers/qmlconvertation.h \
    Core/ichange.h \
    Core/changes.h \
    Network/server_response.h \
    Network/authentication_manager.h \
    Network/roosha_service_connector.h \
    Network/network_manager.h \
    Test/Network/translations_test_slot_holder.h \
    Core/world.h \
    Test/Core/changes/worldtest.h \
    GUI/cardcreationcontroller.h \
    GUI/cardlistcontroller.h \
    Helpers/protobuf_converter.h \
    GUI/authentication_controller.h \
    Test/Core/changes/worldtest.h \
    GUI/system_tray.h

# protobuf generated files
SOURCES += Network/Proto/commons.pb.cc \
    Network/Proto/roosha_service.pb.cc \
    Network/Proto/roosha_service.grpc.pb.cc \
    Network/Proto/changes.pb.cc \

HEADERS += Network/Proto/commons.pb.h \
    Network/Proto/roosha_service.pb.h \
    Network/Proto/roosha_service.grpc.pb.h \
    Network/Proto/changes.pb.h \

#-------------------------------

DISTFILES += \
    codeStyle.astylerc
