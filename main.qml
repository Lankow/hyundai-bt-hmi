import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 2.15
import com.voknal.gauge 1.0

Window {
    // flags: Qt.FramelessWindowHint

    property int displayWidth: 800
    property int displayHeight: 480

    width: displayWidth
    height: displayHeight

    minimumWidth: displayWidth
    minimumHeight: displayHeight

    maximumWidth: displayWidth
    maximumHeight: displayHeight

    visible: true
    title: qsTr("CAR-BT-HMI")
    color:"#141414"

    // Vignette Effect Layer
    Canvas {
        anchors.fill: parent
        opacity: 0.7
        onPaint: {
            var ctx = getContext("2d");
            var gradient = ctx.createRadialGradient(width / 2, height / 2, 0, width / 2, height / 2, Math.max(width, height) / 2);

            gradient.addColorStop(0.7, "transparent");
            gradient.addColorStop(1.0, "black");
            ctx.fillStyle = gradient;
            ctx.fillRect(0, 0, width, height);
        }
    }

    Text{
        objectName: "clock"
        text: "--:--"
        color: "#fff"
        font.pointSize: 30
        font.family: "Orbitron"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 30
    }

    Text{
        objectName: "odometer"
        text: "---- Km"
        color: "#fff"
        font.pointSize: 30
        font.family: "Orbitron"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
    }

    Gauge{
        objectName: "speedGauge"
        value: value
        unit: "Km/h"
        size: 350
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 30
        width: size
        height: size
        minValue: 0
        maxValue: 200
    }

    Gauge{
        objectName: "rpmGauge"
        value: value
        unit: "RPM"
        size: 350
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 30
        width: size
        height: size
        minValue: 0
        maxValue: 5000
    }
}
