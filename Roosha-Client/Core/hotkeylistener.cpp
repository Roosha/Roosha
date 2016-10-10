#include "Core/hotkeylistener.h"
#include <QEventLoop>
#include <QApplication>

HotkeyListener::HotkeyListener() {
    qhk = new QHotkey(QKeySequence("ctrl+alt+Q"), true);
}

HotkeyListener::~HotkeyListener() {
    delete qhk;
}

void HotkeyListener::startListen() {
    emit newWord("Hello");

    QEventLoop l;

      connect(qhk, SIGNAL(activated()), this, SLOT(newUserAction()));

    l.exec();
}

void HotkeyListener::newUserAction() {
    emit newWord("I am from the hotkey");
}
