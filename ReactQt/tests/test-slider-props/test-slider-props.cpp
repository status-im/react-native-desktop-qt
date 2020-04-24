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

class TestSliderProps : public ReactPropertyTestCase {
    Q_OBJECT

private slots:

    virtual void initTestCase() override;

protected:
    virtual QQuickItem* control() const override;
    virtual QVariantMap propValues() const override;
};

QQuickItem* TestSliderProps::control() const {
    return topJSComponent();
}

void TestSliderProps::initTestCase() {
    ReactPropertyTestCase::initTestCase();
    loadQML(QUrl("qrc:/TestSliderProps.qml"));
    waitAndVerifyJsAppStarted();
    ReactPropertyTestCase::init();
}

QVariantMap TestSliderProps::propValues() const {
    return {{"p_value", 0.5},
            {"p_step", 0.1},
            {"p_minimumValue", 0},
            {"p_maximumValue", 1.0},
            {"p_minimumTrackTintColor", "green"},
            {"p_maximumTrackTintColor", "green"},
            {"p_disabled", false},
            {"p_thumbTintColor", "green"},
            {"p_testID", "testSlider"}};
}

QTEST_MAIN(TestSliderProps)
#include "test-slider-props.moc"
