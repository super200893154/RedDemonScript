import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    width: 800
    height: 600
    visible: true
    title: qsTr("RedDemonScript - 账号密码加密测试")

    color: "#1e1e1e"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // 标题
        Label {
            text: "LogManager 日志系统测试"
            font.pixelSize: 24
            font.bold: true
            color: "#e0e0e0"
            Layout.alignment: Qt.AlignHCenter
        }

        // 账号选择
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: "账号:"
                color: "#b0b0b0"
            }

            TextField {
                id: accountInput
                text: "测试账号1"
                placeholderText: "输入账号名"
                Layout.fillWidth: true
                color: "#e0e0e0"
                background: Rectangle {
                    color: "#2d2d2d"
                    radius: 4
                }
            }
        }

        // 日志消息输入
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: "消息:"
                color: "#b0b0b0"
            }

            TextField {
                id: messageInput
                text: "这是一条测试日志消息"
                placeholderText: "输入日志消息"
                Layout.fillWidth: true
                color: "#e0e0e0"
                background: Rectangle {
                    color: "#2d2d2d"
                    radius: 4
                }
            }
        }

        // 日志级别按钮
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: "日志级别:"
                color: "#b0b0b0"
            }

            Button {
                text: "INFO"
                onClicked: {
                    LogManager.logInfo(accountInput.text, messageInput.text)
                }
                background: Rectangle {
                    color: parent.down ? "#1a5a1a" : parent.hovered ? "#2a8a2a" : "#1a7a1a"
                    radius: 4
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                text: "WARN"
                onClicked: {
                    LogManager.logWarn(accountInput.text, messageInput.text)
                }
                background: Rectangle {
                    color: parent.down ? "#8a6a1a" : parent.hovered ? "#aa8a2a" : "#aa7a1a"
                    radius: 4
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                text: "ERROR"
                onClicked: {
                    LogManager.logError(accountInput.text, messageInput.text)
                }
                background: Rectangle {
                    color: parent.down ? "#8a1a1a" : parent.hovered ? "#aa2a2a" : "#aa1a1a"
                    radius: 4
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // 日志显示区域
        Label {
            text: "日志记录:"
            color: "#b0b0b0"
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#2d2d2d"
            radius: 4

            ScrollView {
                anchors.fill: parent
                anchors.margins: 5

                ListView {
                    id: logListView
                    model: logModel
                    delegate: logDelegate
                    clip: true

                    ListModel {
                        id: logModel
                    }

                    Component {
                        id: logDelegate

                        Rectangle {
                            width: logListView.width
                            height: logText.implicitHeight + 10
                            color: index % 2 === 0 ? "#2d2d2d" : "#353535"

                            Text {
                                id: logText
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.margins: 5
                                text: model.logText
                                color: {
                                    if (model.logText.indexOf("[ERROR]") >= 0) return "#ff6b6b"
                                    if (model.logText.indexOf("[WARN]") >= 0) return "#ffd93d"
                                    return "#6bcb77"
                                }
                                font.family: "Consolas, Monaco, monospace"
                                font.pixelSize: 12
                                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                            }
                        }
                    }
                }
            }
        }

        // 操作按钮
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "清空日志显示"
                onClicked: {
                    logModel.clear()
                }
                background: Rectangle {
                    color: parent.down ? "#4a4a4a" : parent.hovered ? "#5a5a5a" : "#4a4a4a"
                    radius: 4
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                text: "加载历史日志"
                onClicked: {
                    var logs = LogManager.getRecentLogs(accountInput.text, 50)
                    logModel.clear()
                    for (var i = 0; i < logs.length; i++) {
                        logModel.append({ logText: logs[i] })
                    }
                }
                background: Rectangle {
                    color: parent.down ? "#4a4a4a" : parent.hovered ? "#5a5a5a" : "#4a4a4a"
                    radius: 4
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // ========== 账号密码加密测试区域 ==========
        Rectangle {
            Layout.fillWidth: true
            height: 2
            color: "#555555"
        }

        Label {
            text: "账号密码加密测试"
            font.pixelSize: 18
            font.bold: true
            color: "#e0e0e0"
        }

        // 账号信息输入
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: "账号:"
                color: "#b0b0b0"
            }

            TextField {
                id: testAccountInput
                text: "testUser"
                placeholderText: "输入账号"
                Layout.fillWidth: true
                color: "#e0e0e0"
                background: Rectangle {
                    color: "#2d2d2d"
                    radius: 4
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Label {
                text: "密码:"
                color: "#b0b0b0"
            }

            TextField {
                id: testPasswordInput
                text: "mySecretPassword123"
                placeholderText: "输入密码"
                Layout.fillWidth: true
                color: "#e0e0e0"
                echoMode: TextInput.Password
                background: Rectangle {
                    color: "#2d2d2d"
                    radius: 4
                }
            }
        }

        // 操作按钮
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                text: "保存账号"
                onClicked: {
                    var accountObj = {
                        "username": testAccountInput.text,
                        "password": testPasswordInput.text
                    }
                    var success = ConfigManager.addAccount(accountObj)
                    if (success) {
                        testResultText.text = "✓ 账号保存成功！密码已加密存储。"
                        LogManager.logInfo("system", "测试：账号 " + testAccountInput.text + " 已保存，密码已加密")
                    } else {
                        testResultText.text = "✗ 账号保存失败！"
                    }
                }
                background: Rectangle {
                    color: parent.down ? "#1a5a8a" : parent.hovered ? "#2a7aaa" : "#1a6a9a"
                    radius: 4
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                text: "读取账号"
                onClicked: {
                    var accounts = ConfigManager.accounts()
                    var result = "账号列表：\n"
                    for (var i = 0; i < accounts.length; i++) {
                        var acc = accounts[i]
                        result += "\n[" + i + "] 用户名: " + acc.username + "\n"
                        result += "    密码: " + acc.password + "\n"
                        if (acc.passwordEncrypted) {
                            result += "    已加密: 是\n"
                        }
                    }
                    testResultText.text = result
                    LogManager.logInfo("system", "测试：读取了 " + accounts.length + " 个账号")
                }
                background: Rectangle {
                    color: parent.down ? "#4a4a4a" : parent.hovered ? "#5a5a5a" : "#4a4a4a"
                    radius: 4
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                text: "查看加密数据"
                onClicked: {
                    // 直接读取配置文件查看加密后的密码
                    var accounts = ConfigManager.accounts()
                    var configPath = ConfigManager.configPath
                    testResultText.text = "配置路径: " + configPath + "\n\n请手动查看 config.json 文件中的 password 字段"
                    LogManager.logInfo("system", "测试：查看加密数据")
                }
                background: Rectangle {
                    color: parent.down ? "#4a4a4a" : parent.hovered ? "#5a5a5a" : "#4a4a4a"
                    radius: 4
                }
                contentItem: Text {
                    text: parent.text
                    color: "#ffffff"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // 结果显示
        Label {
            text: "测试结果:"
            color: "#b0b0b0"
        }

        // 显示配置路径
        Label {
            id: configPathLabel
            text: "配置路径: " + ConfigManager.configPath
            color: "#888888"
            font.pixelSize: 10
        }

        Rectangle {
            Layout.fillWidth: true
            height: 80
            color: "#2d2d2d"
            radius: 4

            ScrollView {
                anchors.fill: parent
                anchors.margins: 5

                Text {
                    id: testResultText
                    text: "点击按钮进行测试..."
                    color: "#6bcb77"
                    font.family: "Consolas, Monaco, monospace"
                    font.pixelSize: 12
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }
        }
    }

    // 连接 LogManager 信号
    Connections {
        target: LogManager

        function onLogAppended(account, formattedLog) {
            // 如果日志来自当前选择的账号，则显示
            if (account === accountInput.text) {
                logModel.append({ logText: formattedLog })
                // 自动滚动到底部
                logListView.positionViewAtEnd()
            }
        }
    }

    // 初始日志
    Component.onCompleted: {
        LogManager.logInfo("系统", "日志系统初始化完成")
    }
}