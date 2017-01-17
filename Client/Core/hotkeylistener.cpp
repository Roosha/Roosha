#include <QEventLoop>
#include <QApplication>
#include <QClipboard>

#include "Core/hotkeylistener.h"

HotkeyListener::HotkeyListener(QObject *parent) : QThread(parent) {
    qhk = new QHotkey(QKeySequence("ctrl+alt+Q"), true);
}

HotkeyListener::~HotkeyListener() {
    delete qhk;
}

void HotkeyListener::run() {
//    emit newWord("Hello");

//    QEventLoop l;

    connect(qhk, &QHotkey::activated, this, &HotkeyListener::newUserAction);

//    l.exec();
}

void HotkeyListener::newUserAction() {
    QString userText;
    if (QApplication::clipboard()->supportsSelection()) {
        userText = QApplication::clipboard()->text(QClipboard::Selection);
    } else {
        userText = getTextUsingClipboardSwap();
    }
    if (!userText.isEmpty()) {
        emit newWord(userText);
    }
}

QString HotkeyListener::getTextUsingClipboardSwap() {
    return QString(); // TODO: support Win and Max OS #Roosha-28
}
