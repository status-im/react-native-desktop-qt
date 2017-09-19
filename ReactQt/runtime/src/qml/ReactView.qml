import QtQuick 2.4
import React 0.1 as React

React.Item {
    id: viewRoot

    property var p_transformMatrix;
    property var viewManager: null
    property string p_nativeID

    objectName: p_nativeID

    onP_transformMatrixChanged: viewManager.manageTransforMatrix(p_transformMatrix, viewRoot)
}
