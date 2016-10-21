TEMPLATE = app

QT += qml quick widgets core

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
    Network/translation_service.cpp \
    Network/network_manager.cpp \
    Test/Network/translations_test_slot_holder.cpp \
    Core/applicationdaemon.cpp \
    Core/changes.cpp \
    Core/cardbuilder.cpp

RESOURCES += \
    QML/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = QML/

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
    Core/ichange.h \
    Core/changes.h \
    Core/cardbuilder.h \
    Network/translation_service.h \
    Network/async_client_call.h \
    Network/network_manager.h \
    Test/Network/translations_test_slot_holder.h

# protobuf generated files
SOURCES += Network/Proto/commons.pb.cc \
    Network/Proto/translation_service.pb.cc \
    Network/Proto/translation_service.grpc.pb.cc

HEADERS += Network/Proto/commons.pb.h \
    Network/Proto/translation_service.pb.h \
    Network/Proto/translation_service.grpc.pb.h

OTHER_FILES += Network/Proto/commons.proto \
    Network/Proto/translation_service.proto
#-------------------------------

DISTFILES += \
    codeStyle.astylerc
