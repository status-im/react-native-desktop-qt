
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QJsonObject>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include <QDebug>

#include "attachedproperties.h"
#include "bridge.h"
#include "cameramanager.h"
#include "propertyhandler.h"
#include "utilities.h"

enum BarcodeTypes {
    TypeUPCECode,
    TypeCode39Code,
    TypeCode39Mod43Code,
    TypeEAN13Code,
    TypeEAN8Code,
    TypeCode93Code,
    TypeCode128Code,
    TypePDF417Code,
    TypeQRCode,
    TypeAztecCode
};

const QVariantMap barcodeTypes{{"upce", TypeUPCECode},
                               {"code39", TypeCode39Code},
                               {"code39mod43", TypeCode39Mod43Code},
                               {"ean13", TypeEAN13Code},
                               {"ean8", TypeEAN8Code},
                               {"code93", TypeCode93Code},
                               {"code128", TypeCode128Code},
                               {"pdf417", TypePDF417Code},
                               {"qr", TypeQRCode},
                               {"aztec", TypeAztecCode}};

enum CameraAspect { CameraAspectStretch, CameraAspectFill, CameraAspectFit };

enum CameraCaptureSessionPreset {
    CameraCaptureSessionPresetLow,
    CameraCaptureSessionPresetMedium,
    CameraCaptureSessionPresetHigh,
    CameraCaptureSessionPresetPhoto,
    CameraCaptureSessionPreset480p,
    CameraCaptureSessionPreset720p,
    CameraCaptureSessionPreset1080p
};

enum CameraCaptureMode { CameraCaptureModeStill, CameraCaptureModeVideo };

enum CameraCaptureTarget {
    CameraCaptureTargetMemory,
    CameraCaptureTargetDisk,
    CameraCaptureTargetTemp,
    CameraCaptureTargetCameraRoll
};

enum CameraOrientation {
    CameraOrientationAuto,
    CameraOrientationLandscapeLeft,
    CameraOrientationLandscapeRight,
    CameraOrientationPortrait,
    CameraOrientationPortraitUpsideDown
};

enum CameraType { CameraTypeFront, CameraTypeBack };

enum CameraFlashMode { CameraFlashModeOff, CameraFlashModeOn, CameraFlashModeAuto };

enum CameraTorchMode { CameraTorchModeOff, CameraTorchModeOn, CameraTorchModeAuto };

class CameraManagerPrivate {};

CameraManager::CameraManager(QObject* parent) : ViewManager(parent), d_ptr(new CameraManagerPrivate) {}

CameraManager::~CameraManager() {}

ViewManager* CameraManager::viewManager() {
    return this;
}

QString CameraManager::moduleName() {
    return "RCTCameraManager";
}

QVariantMap CameraManager::constantsToExport() {
    QVariantMap constants;

    QVariantMap aspectProps{{"stretch", CameraAspectStretch}, {"fill", CameraAspectFill}, {"fit", CameraAspectFit}};
    QVariantMap typeProps{{"front", CameraTypeFront}, {"back", CameraTypeBack}};
    QVariantMap captureModeProps{{"still", CameraCaptureModeStill}, {"video", CameraCaptureModeVideo}};
    QVariantMap captureQualityProps{{"low", CameraCaptureSessionPresetLow},
                                    {"AVCaptureSessionPresetLow", CameraCaptureSessionPresetLow},
                                    {"medium", CameraCaptureSessionPresetMedium},
                                    {"AVCaptureSessionPresetMedium", CameraCaptureSessionPresetMedium},
                                    {"high", CameraCaptureSessionPresetHigh},
                                    {"AVCaptureSessionPresetHigh", CameraCaptureSessionPresetHigh},
                                    {"photo", CameraCaptureSessionPresetPhoto},
                                    {"AVCaptureSessionPresetPhoto", CameraCaptureSessionPresetPhoto},
                                    {"480p", CameraCaptureSessionPreset480p},
                                    {"AVCaptureSessionPreset640x480", CameraCaptureSessionPreset480p},
                                    {"720p", CameraCaptureSessionPreset720p},
                                    {"AVCaptureSessionPreset1280x720", CameraCaptureSessionPreset720p},
                                    {"1080p", CameraCaptureSessionPreset1080p},
                                    {"AVCaptureSessionPreset1920x1080", CameraCaptureSessionPreset1080p}};

    QVariantMap captureTargetProps{{"memory", CameraCaptureTargetMemory},
                                   {"disk", CameraCaptureTargetDisk},
                                   {"temp", CameraCaptureTargetTemp},
                                   {"cameraRoll", CameraCaptureTargetCameraRoll}};
    QVariantMap orientationProps{{"auto", CameraOrientationAuto},
                                 {"landscapeLeft", CameraOrientationLandscapeLeft},
                                 {"landscapeRight", CameraOrientationLandscapeRight},
                                 {"portrait", CameraOrientationPortrait},
                                 {"portraitUpsideDown", CameraOrientationPortraitUpsideDown}};
    QVariantMap flashModeProps{{"off", CameraFlashModeOff}, {"on", CameraFlashModeOn}, {"auto", CameraFlashModeAuto}};
    QVariantMap torchModeProps{{"off", CameraTorchModeOff}, {"on", CameraTorchModeOn}, {"auto", CameraTorchModeAuto}};

    constants.insert("Aspect", aspectProps);
    constants.insert("Type", typeProps);
    constants.insert("CaptureMode", captureModeProps);
    constants.insert("CaptureQuality", captureQualityProps);
    constants.insert("CaptureTarget", captureTargetProps);
    constants.insert("Orientation", orientationProps);
    constants.insert("FlashMode", flashModeProps);
    constants.insert("TorchMode", torchModeProps);
    constants.insert("BarCodeType", barcodeTypes);

    return constants;
}

QString CameraManager::qmlComponentFile() const {

    qDebug() << "!!!!!!!!!!!! requested camera Object";
    return "qrc:/qml/ReactCamera.qml";
}

void CameraManager::configureView(QQuickItem* view) const {

    ViewManager::configureView(view);
    view->setProperty("pickerManager", QVariant::fromValue((QObject*)this));
}

#include "cameramanager.moc"
