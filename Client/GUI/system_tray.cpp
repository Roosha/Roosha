#include "system_tray.h"
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QApplication>
#include <QClipboard>

SystemTray::SystemTray(QWidget *parent) : QWidget(parent) {
    createMenu();

    trayIcon->show();
}

void SystemTray::close() {
    emit closeApplication();
}

void SystemTray::translate() {
    // TODO: this funciton is absolutely the same as function newUserAction in hklistener
    // Maybe it is a good idea to create a special helper
    QString userText;
    if(QApplication::clipboard()->supportsSelection()) {
        userText = QApplication::clipboard()->text(QClipboard::Selection);
    }
    if(!userText.isEmpty()) {
        emit newWord(userText);
    }
}

void SystemTray::show() {
    emit showMainWidnow();
}

void SystemTray::noConnection() {
#ifdef Q_OS_WIN
    // TODO: make normal OSD in linux
    trayIcon->showMessage(
        tr("Connection problem"),
        tr("Sorry, but now network in unavaliable"),
        QSystemTrayIcon::MessageIcon::Critical,
        5000);
#endif
}

void SystemTray::createMenu() {
    translateAction = new QAction(tr("Translate"), this);
    connect(translateAction, &QAction::triggered, this, &SystemTray::translate);

    showAction = new QAction(tr("Show main window"), this);
    connect(showAction, &QAction::triggered, this, &SystemTray::show);

    closeAction = new QAction(tr("Close application"), this);
    connect(closeAction, &QAction::triggered, this, &SystemTray::close);


    trayMenu = new QMenu(this);
    trayMenu->addAction(translateAction);
    trayMenu->addAction(showAction);
    trayMenu->addAction(closeAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayMenu);

    icon = QIcon(":/img/32.png");
    trayIcon->setIcon(icon);
}
