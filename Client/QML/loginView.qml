import QtQuick 2.7
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
            text: credentials.login
        }

        TextField {
            id: password
            height: 100
            Layout.fillWidth: true
            placeholderText: "Password"
            text: credentials.password
            font.pixelSize: 20
            echoMode: TextInput.Password
            focus: true

            onAccepted: {
                controller.sendAuthenticationRequest(login.text, password.text);
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Button {
                id: signUpButton

                Layout.fillWidth: true

                text: qsTr("Sign Up")
                onClicked: {
                    controller.showRegistrateWindow();
                }
            }

            Button {
                id: loginButton

                Layout.fillWidth: true

                text: qsTr("Login")
                onClicked: {
                    controller.sendAuthenticationRequest(login.text, password.text);
                }
            }
        }

        states: [
            State {
                name: "NotAuthenticated"
                when: controller.state_ === AuthController.NotAuthenticated
                PropertyChanges {
                    target: signUpButton
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
                    text: qsTr("Enter your login and password")
                    color: "black"
                }
            },
            State {
                name: "InProgress"
                when: controller.state_ === AuthController.InProgress
                PropertyChanges {
                    target: signUpButton
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
                    text: qsTr("Authenticating...")
                    color: "black"
                }
            },
            State {
                name: "AuthenticationFailure"
                when: controller.state_ === AuthController.AuthenticationFailure
                PropertyChanges {
                    target: signUpButton
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
                    text: qsTr("Authtorisation failed. Try again.")
                    color: "red"
                }
            },
            State {
                name: "AuthenticationSuccess"
                when: controller.state_ === AuthController.AuthenticationSuccess
                PropertyChanges {
                    target: signUpButton
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
                    text: qsTr("Authentication success!")
                    color: "green"
                }
            }

        ]

    }
}
