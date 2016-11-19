#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QWidget>

class SystemTray : public QWidget {
    Q_OBJECT
public:
    SystemTray(QWidget *parent = Q_NULLPTR);

public slots:
    void close();
    void translate();
    void show();

    void noConnection();

signals:
    void closeApplication();
    void newWord(QString word);
    void showMainWidnow();

private:
    void createMenu();

    QAction *showAction;
    QAction *closeAction;
    QAction *translateAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QIcon icon;
};

#endif // SYSTEMTRAY_H
