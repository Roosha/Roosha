#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QWidget>

class SystemTray : public QWidget {
 Q_OBJECT
 public:
    SystemTray(QWidget *parent = Q_NULLPTR);

 public slots:
    void onCloseButtonClicked();
    void onTranslateButtonClicked();
    void onShowButtonClicked();

    void showNoConnectionNotification();

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
