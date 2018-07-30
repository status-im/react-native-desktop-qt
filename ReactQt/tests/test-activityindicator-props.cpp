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
#include "rootview.h"
#include <QTest>
#include <QtQuick/QQuickView>

class TestActivityIndicatorProps : public ReactPropertyTestCase {
    Q_OBJECT

private:
    QQuickItem* activityIndicator();

private slots:

    virtual void initTestCase() override;

protected:
    virtual QQuickItem* control() const override;
    virtual QVariantMap propValues() const override;
};

QQuickItem* TestActivityIndicatorProps::control() const {
    // Even when in JS we have only one <ActivityIndicator> component returned in render(),
    // it is wrapped in two <View> components implicitly, so we have hierarchy in QML:
    // ReactView
    //  |-<View>
    //    |-<View>
    //      |-<ActivityIndicator>

    QList<QQuickItem*> reactViewChilds = rootView()->childItems();
    Q_ASSERT(reactViewChilds.count() == 1);

    QQuickItem* view1 = reactViewChilds[0];
    QList<QQuickItem*> view1Childs = view1->childItems();
    Q_ASSERT(view1Childs.count() == 1);

    QQuickItem* view2 = view1Childs[0];
    QList<QQuickItem*> view2Childs = view2->childItems();
    Q_ASSERT(view2Childs.count() == 1);

    QQuickItem* activityIndicator = view2Childs[0];
    Q_ASSERT(activityIndicator);

    return activityIndicator;
}

void TestActivityIndicatorProps::initTestCase() {
    ReactPropertyTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestActivityIndicatorProps.qml"));
    waitAndVerifyJsAppStarted();
    ReactPropertyTestCase::init();
}

QVariantMap TestActivityIndicatorProps::propValues() const {
    return {{"p_size", "large"}, {"p_color", "#0000FF"}, {"p_animating", true}};
}

QTEST_MAIN(TestActivityIndicatorProps)
#include "test-activityindicator-props.moc"
