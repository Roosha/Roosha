#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QWidget>

class SystemTray : public QWidget {
 Q_OBJECT
 public:
    SystemTray(QWidget *parent = Q_NULLPTR);

    static SystemTray* tray_;
    static void Notify(QString title, QString message, bool warning = false) {
        tray_->trayIcon->showMessage(title, message, warning ? QSystemTrayIcon::Warning : QSystemTrayIcon::Information);
    }

 public slots:
    void onCloseButtonClicked();
    void onTranslateButtonClicked();
    void onShowButtonClicked();
    void onLoginButtonClicked();
    void updateMenu();
    void showNoConnectionNotification();

 signals:
    void closeApplication();
    void newWord(QString word);
    void showMainWindow();
    void showLoginWindow();

 private:
    void createMenu();

    QAction *showAction;
    QAction *closeAction;
    QAction *translateAction;
    QAction *logAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QIcon icon;
};

#endif // SYSTEMTRAY_H
