| React Native                | Canonical | Used by Status | React Native Qt | Implementation details         | Complexity |
|-----------------------------|-----------|----------------|-----------------|--------------------------------|------------|
| **Components**              |           |                |                 |                                |            |
| ActivityIndicator           |           | +              | +/-             |                                |            |
| Button                      |           |                | +/-             |                                |            |
| DatePickerIOS               | ?         |                |                 |                                |            |
| DrawerLayoutAndroid         |           | +              |                 | Drawer QML Type                | medium     |
| FlatList                    |           |                |                 |                                |            |
| Image                       | +/-       | +              | +/-             |                                |            |
| KeyboardAvoidingView        |           | +              |                 | ???? PLACEHOLDER               | minor      |
| ListView                    |           | +              |                 | ListView QML Type              | complex    |
| Modal                       |           | +              | +/-             | Dialog QML Type                | medium     |
| NavigatorIOS                | ?         |                |                 |                                |            |
| Picker                      |           | +              |                 | Tumbler or Combobox QML Type   | medium     |
| PickerIOS                   |           |                |                 |                                |            |
| ProgressBarAndroid          |           |                |                 |                                |            |
| ProgressViewIOS             |           |                |                 |                                |            |
| RefreshControl              |           |                |                 |                                |            |
| ScrollView                  | ?         | +              | +/-             |                                |            |
| SectionList                 |           |                |                 |                                |            |
| SegmentedControlIOS         |           | +              | +/-             |                                |            |
| Slider                      |           |                |                 |                                |            |
| SnapshotViewIOS             |           |                |                 |                                |            |
| StatusBar                   |           | +              |                 | StatusBar QML Type or custom   | medium     |
| Switch                      |           |                |                 |                                |            |
| TabBarIOS                   |           |                |                 |                                |            |
| TabBarIOS.Item              |           |                |                 |                                |            |
| Text                        | ?         | +              | +/-             |                                |            |
| TextInput                   |           | +              | +/-             | TextArea QML Type              | medium     |
| ToolbarAndroid              |           |                |                 |                                |            |
| TouchableHighlight          | ?         | +              |                 |                                | medium++   |
| TouchableNativeFeedback     | ?         |                |                 |                                |            |
| TouchableOpacity            | ?         | +              |                 |                                | medium++   |
| TouchableWithoutFeedback    | ?         | +              |                 |                                | medium++   |
| View                        | ?         | +              | +/-             |                                |            |
| ViewPagerAndroid            |           |                |                 |                                |            |
| VirtualizedList             |           |                |                 |                                |            |
| WebView                     |           | +              |                 | WebView QML Type               | medium++   |
| **APIs**                    |           |                |                 |                                |            |
| AccessibilityInfo           |           |                |                 |                                |            |
| ActionSheetIOS              |           |                |                 |                                |            |
| AdSupportIOS                |           |                |                 |                                |            |
| Alert                       |           |                |                 |                                |            |
| AlertIOS                    |           |                |                 |                                |            |
| Animated                    |           | + (View,Text)  |                 |                                | complex    |
| AppRegistry                 |           | +              | +/-             | Component is JS based          |            |
| AppState                    |           | +              | +/-             |                                |            |
| AsyncStorage                |           |                |                 |                                |            |
| BackAndroid                 |           |                |                 |                                |            |
| BackHandler                 |           |                |                 |                                |            |
| CameraRoll                  |           |                |                 |                                |            |
| Clipboard                   |           | +              |                 | QClipboard                     | minor      |
| DatePickerAndroid           |           |                |                 |                                |            |
| Dimensions                  |           | +              |                 |                                | medium     |
| Easing                      |           |                |                 |                                |            |
| Geolocation                 |           |                |                 |                                |            |
| ImageEditor                 |           |                |                 |                                |            |
| ImagePickerIOS              |           |                |                 |                                |            |
| ImageStore                  |           |                |                 |                                |            |
| InteractionManager          |           |                |                 |                                |            |
| Keyboard                    |           |                |                 |                                |            |
| LayoutAnimation             |           |                |                 |                                |            |
| Linking                     |           | +              |                 | ???? Custom protocol handler   | complex    |
| NetInfo                     |           | +              |                 | QNetworkConfigurationManager:: | complex    |
|                             |           |                |                 | isOnline                       |            |
| PanResponder                |           | +              |                 | MultiPointTouchArea QML Type   | complex    |
| PermissionsAndroid          |           |                |                 |                                |            |
| PixelRatio                  |           |                |                 |                                |            |
| PushNotificationIOS         |           |                |                 |                                |            |
| Settings                    |           |                |                 |                                |            |
| Share                       |           |                |                 |                                |            |
| StatusBarIOS                |           |                |                 |                                |            |
| StyleSheet                  |           |                |                 |                                |            |
| Systrace                    |           |                |                 |                                |            |
| TimePickerAndroid           |           |                |                 |                                |            |
| ToastAndroid                |           |                |                 |                                |            |
| Vibration                   |           |                |                 |                                |            |
| VibrationIOS                |           |                |                 |                                |            |
| Layout Props                |           |                |                 |                                |            |
| Shadow Props                |           |                |                 |                                |            |
| ViewPropTypes               |           |                |                 |                                |            |
| ViewStylePropTypes          |           |                |                 |                                |            |
| TextStylePropTypes          |           |                |                 |                                |            |
| ImageStylePropTypes         |           |                |                 |                                |            |
| **From old React Native: ** |           |                |                 |                                |            |
| Navigator (v0.43)           |           | +              |                 | ???? Backward support in v0.48 |            |
