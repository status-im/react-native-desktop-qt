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

class TestModalProps : public ReactPropertyTestCase {
    Q_OBJECT

private slots:

    virtual void initTestCase() override;

protected:
    virtual QQuickItem* control() const override;
    virtual QVariantMap propValues() const override;
};

QQuickItem* TestModalProps::control() const {
    return topJSComponent();
}

void TestModalProps::initTestCase() {
    ReactPropertyTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestModalProps.qml"));
    waitAndVerifyJsAppStarted();
    ReactPropertyTestCase::init();
}

QVariantMap TestModalProps::propValues() const {
    return {{"p_animationType", "slide"}, {"p_onShow", true}, {"p_transparent", false}};
}

QTEST_MAIN(TestModalProps)
#include "test-modal-props.moc"
