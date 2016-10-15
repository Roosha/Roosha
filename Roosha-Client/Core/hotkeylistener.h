#ifndef HOTKEYLISTENER_H
#define HOTKEYLISTENER_H

#include <QObject>
#include <QThread>
#include <QHotkey>

class HotkeyListener : public QThread {
    Q_OBJECT

 public:
    HotkeyListener(QObject * parent = Q_NULLPTR);
    ~HotkeyListener();

    void run() Q_DECL_OVERRIDE;
public slots:
    void newUserAction();

signals:
    void newWord(QString word);

private:
    QHotkey * qhk;

    QString getTextUsingClipboardSwap();
};

#endif // HOTKEYLISTENER_H
