import QtQuick 2.7
import QtQuick.Controls 2.0
import QtMultimedia 5.8
import My.Filters 1.0

ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: qsTr("Hello World")

    FaceFilter {
        id: faceFilter
        scale: 2
        predictorPath: "D:/jack/qt-workspace/hello-dlib/hello-dlib/shape_predictor_68_face_landmarks.dat"
        onFinished: function (points) {
            console.log("frame finished")
        }
        onFaceDetectStarted: console.log('started face detection')
        onFaceDetectStopped: console.log('stopped face detection')
    }

    Camera {
        id: camera
        deviceId: QtMultimedia.availableCameras[0].deviceId
        viewfinder.resolution: Qt.size(960, 540)
        viewfinder.minimumFrameRate: 30
        viewfinder.maximumFrameRate: 30
    }

    VideoOutput {
        id: viewfinder
        anchors.fill: parent
        source: camera
        filters: [faceFilter]
    }

    MouseArea {
        anchors.fill: parent
        onClicked: function () {
            console.log('clicked');
            if (faceFilter.isActive()) {
                faceFilter.stop();
            } else {
                faceFilter.start();
            }
        }
    }
}
