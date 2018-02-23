import QtQuick 2.9
import QtQuick.Controls 2.2
import React 0.1 as React
import QtMultimedia 5.8

Item {
    id: cameraRoot

    property var cameraManager: null
    property var flexbox: React.Flexbox {control: cameraRoot}

    property int p_aspect: React.CameraManager.CameraAspectFill
    property int p_captureMode: React.CameraManager.CameraCaptureModeStill

    function capture() {
        camera.imageCapture.capture()
    }

    Camera {
        id: camera

        captureMode: getCaptureMode(p_captureMode)

        imageCapture {
            onCaptureFailed: cameraManager.captureFailed(message)
            onImageSaved: cameraManager.imageSaved(path)
        }
    }

    VideoOutput {
        source: camera
        anchors.fill: parent
        focus : visible
        fillMode: aspectToFillMode(p_aspect)
    }

    function aspectToFillMode(aspect) {
        switch (aspect) {
        case React.CameraManager.CameraAspectFill: return VideoOutput.PreserveAspectCrop
        case React.CameraManager.CameraAspectFit: return VideoOutput.PreserveAspectFit
        case React.CameraManager.CameraAspectStretch:  return VideoOutput.Stretch
        default: return VideoOutput.PreserveAspectCrop
        }
    }

    function getCaptureMode(captureMode) {
        switch (captureMode) {
        case React.CameraManager.CameraCaptureModeStill: return Camera.CaptureStillImage
        case React.CameraManager.CameraCaptureModeVideo: return Camera.CaptureVideo
        default: return Camera.CaptureStillImage
        }
    }
}
