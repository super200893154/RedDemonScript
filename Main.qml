import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root
    width: 800
    height: 600
    visible: true
    title: qsTr("RedDemonScript - 日志系统测试")

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