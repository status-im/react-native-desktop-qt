
### How `Yoga` engine works
`Yoga` engine has api that works with "nodes". Every `YGNode` has a set of properties. Some of them used to setup layout settings, some - to read the calculated position of the node.

Every `YGNode` has a set of properties that control the layout of node (like minWidth, alignment, justification, etc.) and set of properties that contain calculated layout results (like x, y, width, height). You can join nodes in hierarchy and `calculate` it. `Yoga` engine will read "controlling" properties and save the result in "position" properties.

### How `Yoga` engine used in `React Native Desktop`
A lot of components in `React Native` have layout properties that are almost identical to properties in `Yoga`.
We have a `Flexbox` object that is wrapper to `YGNode`. It is responsible for getting layout props from JS side and turning them into QML position properties. Here is the process:
1. We receive a set of layout properties from JS side.
2. `ReactNativeHander` search for `Flexbox` object for control and set all the layout properties to `Flexbox`.
3. Whenever `Flexbox` gets a property update, it passes the value to internal `YGNode` object.
4. Whenever `UIManager` receives update on component hierarchy (add/delete/move), it also requests `Flexbox` to update hierarchy of `YGNode`s
5. When `RootView` is ready, it triggers recalculation of `Flexbox` hierarchy
6. When calculation completed, `Flexbox` read results from `YGNode`s and assign them to QML controls.
