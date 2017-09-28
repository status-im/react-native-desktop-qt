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

class TestButtonProps : public ReactPropertyTestCase {
    Q_OBJECT

private:
    QQuickItem* qmlImage();

private slots:

    virtual void initTestCase() override;

protected:
    virtual QQuickItem* control() const override;
    virtual QVariantMap propValues() const override;
};

QQuickItem* TestButtonProps::control() const {
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

    QQuickItem* button = viewChilds[0];
    Q_ASSERT(button);

    return button;
}

void TestButtonProps::initTestCase() {
    ReactPropertyTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestButtonProps.qml"));
    waitAndVerifyJsAppStarted();
}

QVariantMap TestButtonProps::propValues() const {
    return {{"p_accessibilityLabel", "Accessibility label"},
            {"p_title", "Click me"},
            {"p_color", "red"},
            {"p_disabled", false},
            {"p_onPress", true},
            {"p_testID", "button"}};
}

QTEST_MAIN(TestButtonProps)
#include "test-button-props.moc"
