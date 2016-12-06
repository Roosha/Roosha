import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import roosha.controllers 1.0

Rectangle {
    id: root
    width: 300
    height: 200

    ColumnLayout {
        id: credsInput
        anchors.fill: parent

        spacing: 5

        Image {
            id: logo
            source: "qrc:/img/64.png"
            anchors.horizontalCenter: parent.horizontalCenter
            visible: true
        }

        BusyIndicator {
            id: indicator
            anchors.horizontalCenter: parent.horizontalCenter
            running: true
            visible: false
        }

        Label {
            id: infoLabel
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TextField {
            id: login
            height: 100
            Layout.fillWidth: true
            placeholderText: "Username"
            font.pixelSize: 20
        }

        TextField {
            id: password
            height: 100
            Layout.fillWidth: true
            placeholderText: "Password"
            font.pixelSize: 20
            echoMode: TextInput.Password
            onAccepted: {
                controller.sendRegistrateRequest(login.text, password.text);
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Button {
                id: signInButton

                Layout.fillWidth: true

                text: qsTr("Sign In")
                onClicked: {
                    controller.showLoginWindow();
                }
            }

            Button {
                id: loginButton

                Layout.fillWidth: true

                text: qsTr("Register")
                onClicked: {
                    controller.sendRegistrateRequest(login.text, password.text);
                }
            }
        }

        states: [
            State {
                name: "NotAuthenticated"
                when: controller.state_ === AuthController.NotAuthenticated
                PropertyChanges {
                    target: signInButton
                    enabled: true
                }
                PropertyChanges {
                    target: loginButton
                    enabled: true
                }
                PropertyChanges {
                    target: login
                    enabled: true
                }
                PropertyChanges {
                    target: password
                    enabled: true
                }
                PropertyChanges {
                    target: logo
                    visible: true
                }
                PropertyChanges {
                    target: indicator
                    visible: false
                }
                PropertyChanges {
                    target: infoLabel
                    text: qsTr("Enter your new login and password")
                    color: "black"
                }
            },
            State {
                name: "InProgress"
                when: controller.state_ === AuthController.InProgress
                PropertyChanges {
                    target: signInButton
                    enabled: false
                }
                PropertyChanges {
                    target: loginButton
                    enabled: false
                }
                PropertyChanges {
                    target: login
                    enabled: false
                }
                PropertyChanges {
                    target: password
                    enabled: false
                }
                PropertyChanges {
                    target: logo
                    visible: false
                }
                PropertyChanges {
                    target: indicator
                    visible: true
                }
                PropertyChanges {
                    target: infoLabel
                    text: qsTr("Registration...")
                    color: "black"
                }
            },
            State {
                name: "AuthenticationFailure"
                when: controller.state_ === AuthController.AuthenticationFailure
                PropertyChanges {
                    target: signInButton
                    enabled: true
                }
                PropertyChanges {
                    target: loginButton
                    enabled: true
                }
                PropertyChanges {
                    target: login
                    enabled: true
                }
                PropertyChanges {
                    target: password
                    enabled: true
                }
                PropertyChanges {
                    target: logo
                    visible: true
                }
                PropertyChanges {
                    target: indicator
                    visible: false
                }
                PropertyChanges {
                    target: infoLabel
                    text: qsTr("Registration failed. Try again.")
                    color: "red"
                }
            },
            State {
                name: "AuthenticationSuccess"
                when: controller.state_ === AuthController.AuthenticationSuccess
                PropertyChanges {
                    target: signInButton
                    enabled: false
                }
                PropertyChanges {
                    target: loginButton
                    enabled: false
                }
                PropertyChanges {
                    target: login
                    enabled: false
                }
                PropertyChanges {
                    target: password
                    enabled: false
                }
                PropertyChanges {
                    target: logo
                    visible: true
                }
                PropertyChanges {
                    target: indicator
                    visible: false
                }
                PropertyChanges {
                    target: infoLabel
                    text: qsTr("Registration success!")
                    color: "green"
                }
            }

        ]

    }
}
