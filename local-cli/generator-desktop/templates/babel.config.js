const defaultResolvePath = require('babel-plugin-module-resolver').resolvePath;


const rnToRnd = new Map([
  // imports in react-native
  ["../Utilities/Platform",     "../../../react-native-desktop-qt/Libraries/Utilities/Platform"],
  ["./Image",                   "../../../react-native-desktop-qt/Libraries/Image/Image"],
  ["../../Image/Image",         "../../../../react-native-desktop-qt/Libraries/Image/Image"],
  ["../../../Image/Image",      "../../../../../react-native-desktop-qt/Libraries/Image/Image"],
  ["../../Utilities/Platform",  "../../../../react-native-desktop-qt/Libraries/Utilities/Platform"],
  ["./Platform",  "../../../react-native-desktop-qt/Libraries/Utilities/Platform"],
  ["../Components/View/PlatformViewPropTypes",  "../../../react-native-desktop-qt/Libraries/Components/View/PlatformViewPropTypes"],
  ["../Utilities/BackHandler",  "../../../react-native-desktop-qt/Libraries/Utilities/BackHandler"],
  ["./RCTNetworking",                   "../../../react-native-desktop-qt/Libraries/Network/RCTNetworking"],
  ["ScrollView", "../../../react-native-desktop-qt/Libraries/Components/ScrollView/ScrollView"],
  ["../Components/ScrollView/ScrollView", "../../../react-native-desktop-qt/Libraries/Components/ScrollView/ScrollView"],
  ["../../Components/ScrollView/ScrollView", "../../../../react-native-desktop-qt/Libraries/Components/ScrollView/ScrollView"],
  ["../../../Components/ScrollView/ScrollView", "../../../../../react-native-desktop-qt/Libraries/Components/ScrollView/ScrollView"],
]);

const rndToRn = new Map([
  // imports in react-native-desktop-qt
  ["../../Animated/src/AnimatedImplementation", "../../../../react-native/Libraries/Animated/src/AnimatedImplementation"],
  ["../../Renderer/shims/ReactNative", "../../../../react-native/Libraries/Renderer/shims/ReactNative"],
  ["../ScrollResponder", "../../../../react-native/Libraries/Components/ScrollResponder"],
  ["./ScrollViewStickyHeader", "../../../../react-native/Libraries/Components/ScrollView/ScrollViewStickyHeader"],
  ["./processDecelerationRate", "../../../../react-native/Libraries/Components/ScrollView/processDecelerationRate"],
  ["../../StyleSheet/StyleSheet", "../../../../react-native/Libraries/StyleSheet/StyleSheet"],
  ["../View/View", "../../../../react-native/Libraries/Components/View/View"],
  ["../../Utilities/dismissKeyboard", "../../../../react-native/Libraries/Utilities/dismissKeyboard"],
  ["../../StyleSheet/flattenStyle", "../../../../react-native/Libraries/StyleSheet/flattenStyle"],
  ["../../ReactNative/requireNativeComponent", "../../../../react-native/Libraries/ReactNative/requireNativeComponent"],
  ["../../Image/resolveAssetSource", "../../../../react-native/Libraries/Image/resolveAssetSource"],
  ["../../StyleSheet/splitLayoutProps", "../../../../react-native/Libraries/StyleSheet/splitLayoutProps"],
  ["../../DeprecatedPropTypes/DeprecatedColorPropType", "../../../../react-native/Libraries/DeprecatedPropTypes/DeprecatedColorPropType"],
  ["../../DeprecatedPropTypes/DeprecatedViewPropTypes", "../../../../react-native/Libraries/DeprecatedPropTypes/DeprecatedViewPropTypes"],
  ["../../vendor/document/selection/DocumentSelectionState", "../../../../react-native/Libraries/vendor/document/selection/DocumentSelectionState"],
  ["../../Renderer/shims/NativeMethodsMixin", "../../../../react-native/Libraries/Renderer/shims/NativeMethodsMixin"],
  ["../../Text/Text", "../../../../react-native/Libraries/Text/Text"],
  ["../../Text/TextAncestor", "../../../../react-native/Libraries/Text/TextAncestor"],
  ["../BatchedBridge/NativeModules", "../../../react-native/Libraries/BatchedBridge/NativeModules"],
  ["./TextInputState", "../../../../react-native/Libraries/Components/TextInput/TextInputState"],
  ["../Touchable/TouchableWithoutFeedback", "../../../../react-native/Libraries/Components/Touchable/TouchableWithoutFeedback"],
  ["./UIManagerProperties", "../../../react-native/Libraries/ReactNative/UIManagerProperties"],
  ["../Utilities/defineLazyObjectProperty", "../../../react-native/Libraries/Utilities/defineLazyObjectProperty"],
  ["../../ReactNative/UIManager", "../../../../react-native/Libraries/ReactNative/UIManager"],




]);

module.exports = {
  presets: ['module:metro-react-native-babel-preset'],
  plugins: [
    [
      'module-resolver',
      {
        root: ['.'],
        extensions: [
          ".ios.js",
          ".android.js",
          ".js",
          '.ios.ts',
          '.android.ts',
          '.ts',
          '.ios.tsx',
          '.android.tsx',
          '.tsx',
          '.jsx',
          '.js',
          '.json',
        ],
        resolvePath(sourcePath, currentFile, opts) {
          // if(sourcePath == "UIManager") {
          //   console.log("---------------------")
          //   console.log("file: ", currentFile)
          //   if(currentFile.includes("react-native-implementation")) {
          //     console.log("--- return path for react-native-implementation")
          //     return "../../../react-native-desktop-qt/Libraries/ReactNative/UIManager"
          //   }
          //   else {
          //     return "../../../../react-native-desktop-qt/Libraries/ReactNative/UIManager"
          //   }
          // }


          let map = currentFile.includes("react-native-desktop-qt") ? rndToRn : rnToRnd;
          if(map.has(sourcePath))
            return map.get(sourcePath);

          return defaultResolvePath(sourcePath, currentFile, opts);
        },
      },
    ],
  ],
};
