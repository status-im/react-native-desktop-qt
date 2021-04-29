import QtQuick 2.4
import React 0.1 as React

React.Item {
    id: viewRoot

    property var p_transformMatrix;
    property var viewManager: null
    property string p_nativeID
    property int p_zIndex: 0
    property string p_pointerEvents: "auto"
    property var flexbox: React.Flexbox {control: viewRoot; viewManager: viewRoot.viewManager}

    objectName: p_nativeID

    onP_transformMatrixChanged: viewManager.manageTransformMatrix(p_transformMatrix, viewRoot)

    z: p_zIndex
}
