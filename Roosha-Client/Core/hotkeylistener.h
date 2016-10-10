#ifndef HOTKEYLISTENER_H
#define HOTKEYLISTENER_H

#include <QObject>
#include <QThread>

class HotkeyListener : public QObject {
    Q_OBJECT

 public:
    HotkeyListener();

    void startListen();

signals:
    void newWord(QString word);
};

#endif // HOTKEYLISTENER_H
