/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "reactpropertytestcase.h"

#include <QDebug>
#include <QTest>
#include <QtQuick/QQuickView>

class TestImageProps : public ReactPropertyTestCase {
    Q_OBJECT

private:
    QQuickItem* qmlImage();

private slots:

    virtual void initTestCase() override;

protected:
    virtual QQuickItem* control() const override;
    virtual QVariantMap propValues() const override;
};

QQuickItem* TestImageProps::control() const {
    return topJSComponent();
}

void TestImageProps::initTestCase() {
    ReactPropertyTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestImageProps.qml"));
    waitAndVerifyJsAppStarted();
    ReactPropertyTestCase::init();
    // wait when image loaded. otherwise we can get crash if app quites while loading in progress.
    waitAndVerifyCondition([=]() { return valueOfProperty("imageReady").toBool(); }, "Image can't load source");
}

QVariantMap TestImageProps::propValues() const {
    return {{"p_testID", "testImage"},
            {"p_onLoadStart", true},
            {"p_onLoadEnd", true},
            {"p_onLoad", true},
            {"p_onError", true},
            {"p_onProgress", true},
            {"p_blurRadius", 30},
            {"p_onLayout", true},
            {"p_resizeMode", "center"}};
}

QTEST_MAIN(TestImageProps)
#include "test-image-props.moc"
