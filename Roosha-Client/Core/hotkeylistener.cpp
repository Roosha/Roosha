#include "Core/hotkeylistener.h"

HotkeyListener::HotkeyListener() {

}

void HotkeyListener::startListen() {
    QThread::currentThread()->sleep(2);
    emit newWord("Hello");
}
