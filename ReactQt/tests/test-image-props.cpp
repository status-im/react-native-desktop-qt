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
#include "reactpropertytestcase.h"
#include "reactview.h"
#include <QDebug>
#include <QSignalSpy>
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
    ReactPropertyTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestImageProps.qml"));
    waitAndVerifyJsAppStarted();
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
