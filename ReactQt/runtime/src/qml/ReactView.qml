import QtQuick 2.4
import React 0.1 as React

React.Item {
    id: root

    property var p_transformMatrix;
    property var viewManager: null

    onP_transformMatrixChanged: viewManager.manageTransforMatrix(p_transformMatrix, root)
}
