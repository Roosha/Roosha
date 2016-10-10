#ifndef HOTKEYLISTENER_H
#define HOTKEYLISTENER_H

#include <QObject>
#include <QThread>
#include <QHotkey>

class HotkeyListener : public QObject {
    Q_OBJECT

 public:
    HotkeyListener();
    ~HotkeyListener();

    void startListen();
public slots:
    void newUserAction();

signals:
    void newWord(QString word);

private:
    QHotkey * qhk;
};

#endif // HOTKEYLISTENER_H
