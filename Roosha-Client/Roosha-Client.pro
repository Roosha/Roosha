TEMPLATE = app

QT += qml quick widgets core quickwidgets quickcontrols2
CONFIG += c++11

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
    Core/applicationdaemon.cpp \
    Core/translation.cpp \
    GUI/translationcontroller.cpp \
    Helpers/qmlconvertation.cpp

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
    Core/idiff.h \
    GUI/guimanager.h \
    Helpers/bootstrap.h \
    Helpers/configuremanager.h \
    DB/dbmanager.h \
    Network/networkmanager.h \
    Core/applicationdaemon.h \
    Core/translation.h \
    GUI/translationcontroller.h \
    Helpers/qmlconvertation.h
