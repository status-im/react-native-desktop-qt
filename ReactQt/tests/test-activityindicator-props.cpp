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
#include "reacttestcase.h"
#include "reactview.h"
#include <QDebug>
#include <QSignalSpy>
#include <QTest>
#include <QtQuick/QQuickView>

class TestActivityIndicatorProps : public ReactTestCase {
    Q_OBJECT

private:
    QQuickItem* activityIndicator();

private slots:

    void initTestCase();
    void cleanupTestCase();

    void checkSizeProp();
    void checkColorProp();
    void checkAnimatingProp();

private:
    QVariant valueOfProperty(const QString& propertyName);
};

QQuickItem* TestActivityIndicatorProps::activityIndicator() {
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
    ReactTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestActivityIndicatorProps.qml"));
    waitAndVerifyJsAppStarted();
}

void TestActivityIndicatorProps::cleanupTestCase() {
    ReactTestCase::cleanupTestCase();
}

QVariant TestActivityIndicatorProps::valueOfProperty(const QString& propertyName) {
    return activityIndicator()->property(propertyName.toStdString().c_str());
}

void TestActivityIndicatorProps::checkSizeProp() {
    QCOMPARE(valueOfProperty("p_size").toString(), QString("large"));
}

void TestActivityIndicatorProps::checkColorProp() {
    QCOMPARE(valueOfProperty("p_color").toString(), QString("#0000FF"));
}

void TestActivityIndicatorProps::checkAnimatingProp() {
    QCOMPARE(valueOfProperty("p_size").toBool(), true);
}

QTEST_MAIN(TestActivityIndicatorProps)
#include "test-activityindicator-props.moc"
