/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

import QtQuick 2.4
import React 0.1 as React

Rectangle {
    id: root
    width: 640; height: 480;

    React.RootView {
        objectName: "rootView"
        anchors.fill: parent

        moduleName: "TestTextInputProps"
        codeLocation: "http://localhost:8081/ReactQt/tests/JS/TestTextInputProps.bundle?platform=ubuntu&dev=true"
    }
}
