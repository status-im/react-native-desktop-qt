/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactimagemanager.h"
#include "reactpropertyhandler.h"
#include "reacttestcase.h"
#include "reactview.h"
#include <QDebug>
#include <QSignalSpy>
#include <QTest>
#include <QtQuick/QQuickView>

class TestImageProps : public ReactTestCase {
    Q_OBJECT

private:
    QQuickItem* qmlImage();

private slots:

    void initTestCase();
    void cleanupTestCase();
    void checkImageLoaded();

    void checkTestIDProp();
    void checkOnLoadStart();
    void checkOnLoadEnd();
    void checkOnLoad();
    void checkOnError();
    void checkOnProgress();
    void checkBlurRadius();
    void checkOnLayout();
    void checkResizeMode();

private:
    QVariant valueOfProperty(const QString& propertyName);
    ReactImageManager m_ImageManager;
};

QQuickItem* TestImageProps::qmlImage() {
    // Even when in JS we have only one <Image> component returned in render(),
    // it is wrapped in <View> component implicitly, so we have hierarchy in QML:
    // ReactView
    //  |-<View>
    //    |-<Image>

    QList<QQuickItem*> reactViewChilds = rootView()->childItems();
    Q_ASSERT(reactViewChilds.count() == 1);

    QQuickItem* view = reactViewChilds[0];
    QList<QQuickItem*> viewChilds = view->childItems();
    Q_ASSERT(viewChilds.count() == 1);

    QQuickItem* image = viewChilds[0];
    Q_ASSERT(image);

    return image;
}

void TestImageProps::initTestCase() {
    ReactTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestImageProps.qml"));
    waitAndVerifyJsAppStarted();
}

void TestImageProps::cleanupTestCase() {
    ReactTestCase::cleanupTestCase();
}

QVariant TestImageProps::valueOfProperty(const QString& propertyName) {
    return qmlImage()->property(propertyName.toStdString().c_str());
}

void TestImageProps::checkImageLoaded() {
    waitAndVerifyCondition([=]() { return valueOfProperty("imageReady").toBool(); }, "Image can't load source");
    QCOMPARE(valueOfProperty("imageReady").toBool(), true);
}

void TestImageProps::checkTestIDProp() {
    QCOMPARE(valueOfProperty("p_testID").toString(), QString("testImage"));
}

void TestImageProps::checkOnLoadStart() {
    QCOMPARE(valueOfProperty("p_onLoadStart").toBool(), true);
}

void TestImageProps::checkOnLoadEnd() {
    QCOMPARE(valueOfProperty("p_onLoadEnd").toBool(), true);
}

void TestImageProps::checkOnLoad() {
    QCOMPARE(valueOfProperty("p_onLoad").toBool(), true);
}

void TestImageProps::checkOnError() {
    QCOMPARE(valueOfProperty("p_onError").toBool(), true);
}

void TestImageProps::checkOnProgress() {
    QCOMPARE(valueOfProperty("p_onProgress").toBool(), true);
}

void TestImageProps::checkBlurRadius() {
    QCOMPARE(valueOfProperty("p_blurRadius").toInt(), 30);
}

void TestImageProps::checkOnLayout() {
    QCOMPARE(valueOfProperty("p_onLayout").toBool(), true);
}

void TestImageProps::checkResizeMode() {
    QCOMPARE(valueOfProperty("p_resizeMode").toString(), QString("center"));
}

QTEST_MAIN(TestImageProps)
#include "test-image-props.moc"
