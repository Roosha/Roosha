TEMPLATE = app

QT += qml quick widgets core

CONFIG += c++11

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
    Network/networkmanager.h \
    Core/applicationdaemon.h \
    Core/ichange.h \
    Core/changes.h \
    Core/cardbuilder.h

DISTFILES += \
    codeStyle.astylerc
