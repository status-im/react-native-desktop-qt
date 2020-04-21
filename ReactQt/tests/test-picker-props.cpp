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
#include <QDebug>
#include <QTest>
#include <QtQuick/QQuickView>

class TestPickerProps : public ReactPropertyTestCase {
    Q_OBJECT

private slots:

    virtual void initTestCase() override;

protected:
    virtual QQuickItem* control() const override;
    virtual QVariantMap propValues() const override;
};

QQuickItem* TestPickerProps::control() const {
    QList<QQuickItem*> reactViewChilds = rootView()->childItems();
    Q_ASSERT(reactViewChilds.count() == 1);

    QQuickItem* view1 = reactViewChilds[0];
    QList<QQuickItem*> view1Childs = view1->childItems();
    Q_ASSERT(view1Childs.count() == 1);

    QQuickItem* view2 = view1Childs[0];
    QList<QQuickItem*> view2Childs = view2->childItems();
    Q_ASSERT(view2Childs.count() == 1);

    QQuickItem* view3 = view2Childs[0];
    QList<QQuickItem*> view3Childs = view3->childItems();
    Q_ASSERT(view3Childs.count() == 1);

    QQuickItem* pickerControl = view3Childs[0];
    Q_ASSERT(pickerControl);

    return pickerControl;
}

void TestPickerProps::initTestCase() {
    ReactPropertyTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestPickerProps.qml"));
    waitAndVerifyJsAppStarted();
    ReactPropertyTestCase::init();
}

QVariantMap TestPickerProps::propValues() const {
    QVariantList items;
    items.append(QVariantMap{{"label", "red"}, {"value", "red"}});
    items.append(QVariantMap{{"label", "green"}, {"value", "green"}});

    return {{"p_testID", "picker"}, {"p_selected", 1}, {"p_items", items}};
}

QTEST_MAIN(TestPickerProps)
#include "test-picker-props.moc"
