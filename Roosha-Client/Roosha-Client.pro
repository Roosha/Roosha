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
    Core/idiff.cpp \
    GUI/guimanager.cpp \
    Helpers/bootstrap.cpp \
    Helpers/configuremanager.cpp \
    DB/dbmanager.cpp \
    Network/networkmanager.cpp \
    Network/translation_service.cpp

RESOURCES += \
    QML/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = QML/

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    Core/centralcontroller.h \
    Core/hotkeylistener.h \
    Core/card.h \
    Core/dbcard.h \
    Core/idiff.h \
    GUI/guimanager.h \
    Helpers/bootstrap.h \
    Helpers/configuremanager.h \
    DB/dbmanager.h \
    Network/networkmanager.h \
    Network/translation_service.h

# protobuf generated files
SOURCES += Network/Proto/commons.pb.cc \
    Network/Proto/translation_service.pb.cc \
    Network/Proto/translation_service.grpc.pb.cc

HEADERS += Network/Proto/commons.pb.h \
    Network/Proto/translation_service.pb.h \
    Network/Proto/translation_service.grpc.pb.h
#-------------------------------
