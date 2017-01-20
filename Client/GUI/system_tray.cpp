#include "system_tray.h"
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QClipboard>

SystemTray* SystemTray::tray_ = Q_NULLPTR;

SystemTray::SystemTray(QWidget *parent) : QWidget(parent) {
    createMenu();

    trayIcon->show();
    tray_ = this;
}

void SystemTray::onCloseButtonClicked() {
    emit closeApplication();
}

void SystemTray::onTranslateButtonClicked() {
    // TODO: this funciton is absolutely the same as function newUserAction in hklistener
    // Maybe it is a good idea to create a special helper
    QString userText;
    if (QApplication::clipboard()->supportsSelection()) {
        userText = QApplication::clipboard()->text(QClipboard::Selection);
    }
    if (!userText.isEmpty()) {
        emit newWord(userText);
    }
}

void SystemTray::onShowButtonClicked() {
    emit showMainWindow();
}

void SystemTray::onLoginButtonClicked() {
    emit showLoginWindow();
}

void SystemTray::createMenu() {
    translateAction = new QAction(tr("Translate"), this);
    connect(translateAction, &QAction::triggered, this, &SystemTray::onTranslateButtonClicked);

    showAction = new QAction(tr("Show main window"), this);
    connect(showAction, &QAction::triggered, this, &SystemTray::onShowButtonClicked);

    closeAction = new QAction(tr("Close application"), this);
    connect(closeAction, &QAction::triggered, this, &SystemTray::onCloseButtonClicked);

    logAction = new QAction(tr("Login"), this);
    connect(logAction, &QAction::triggered, this, &SystemTray::onLoginButtonClicked);

    trayMenu = new QMenu(this);
    trayMenu->addAction(translateAction);
    trayMenu->addAction(logAction);
    trayMenu->addAction(closeAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayMenu);

    icon = QIcon(":/img/32.png");
    trayIcon->setIcon(icon);
}

/// replace login option with Show Main Window option
void SystemTray::updateMenu() {
    trayMenu->removeAction(logAction);
    trayMenu->insertAction(closeAction, showAction);
}
