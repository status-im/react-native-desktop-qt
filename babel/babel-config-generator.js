var path = require("path");
const defaultResolvePath = require('babel-plugin-module-resolver').resolvePath;

function createRnToRndMap(rndPath) {
  return new Map([
    // imports in react-native
    ["../Utilities/Platform", path.resolve(rndPath, "Libraries/Utilities/Platform")],
    ["./Image", path.resolve(rndPath, "Libraries/Image/Image")],
    ["../../Image/Image", path.resolve(rndPath, "Libraries/Image/Image")],
    ["../../../Image/Image", path.resolve(rndPath, "Libraries/Image/Image")],
    ["../../Utilities/Platform", path.resolve(rndPath, "Libraries/Utilities/Platform")],
    ["./Platform", path.resolve(rndPath, "Libraries/Utilities/Platform")],
    ["../Components/View/PlatformViewPropTypes", path.resolve(rndPath, "Libraries/Components/View/PlatformViewPropTypes")],
    ["../Utilities/BackHandler", path.resolve(rndPath, "Libraries/Utilities/BackHandler")],
    ["./RCTNetworking", path.resolve(rndPath, "Libraries/Network/RCTNetworking")],
    ["ScrollView", path.resolve(rndPath, "Libraries/Components/ScrollView/ScrollView")],
    ["../Components/ScrollView/ScrollView", path.resolve(rndPath, "Libraries/Components/ScrollView/ScrollView")],
    ["../../Components/ScrollView/ScrollView", path.resolve(rndPath, "Libraries/Components/ScrollView/ScrollView")],
    ["../../../Components/ScrollView/ScrollView", path.resolve(rndPath, "Libraries/Components/ScrollView/ScrollView")],
    ["SafeAreaView", path.resolve(rndPath, "Libraries/Components/SafeAreaView/SafeAreaView")],
    ["../../Components/SafeAreaView/SafeAreaView", path.resolve(rndPath, "Libraries/Components/SafeAreaView/SafeAreaView")],
  ]);
}

function createRndToRnMap(rnPath) {
  return new Map([
    // imports in react-native-desktop-qt
    ["../../Animated/src/AnimatedImplementation", path.resolve(rnPath, "Libraries/Animated/src/AnimatedImplementation")],
    ["../../Renderer/shims/ReactNative", path.resolve(rnPath, "Libraries/Renderer/shims/ReactNative")],
    ["../ScrollResponder", path.resolve(rnPath, "Libraries/Components/ScrollResponder")],
    ["./ScrollViewStickyHeader", path.resolve(rnPath, "Libraries/Components/ScrollView/ScrollViewStickyHeader")],
    ["./processDecelerationRate", path.resolve(rnPath, "Libraries/Components/ScrollView/processDecelerationRate")],
    ["../../StyleSheet/StyleSheet", path.resolve(rnPath, "Libraries/StyleSheet/StyleSheet")],
    ["../View/View", path.resolve(rnPath, "Libraries/Components/View/View")],
    ["../../Utilities/dismissKeyboard", path.resolve(rnPath, "Libraries/Utilities/dismissKeyboard")],
    ["../../StyleSheet/flattenStyle", path.resolve(rnPath, "Libraries/StyleSheet/flattenStyle")],
    ["../../ReactNative/requireNativeComponent", path.resolve(rnPath, "Libraries/ReactNative/requireNativeComponent")],
    ["../../Image/resolveAssetSource", path.resolve(rnPath, "Libraries/Image/resolveAssetSource")],
    ["../../StyleSheet/splitLayoutProps", path.resolve(rnPath, "Libraries/StyleSheet/splitLayoutProps")],
    ["../../DeprecatedPropTypes/DeprecatedColorPropType", path.resolve(rnPath, "Libraries/DeprecatedPropTypes/DeprecatedColorPropType")],
    ["../../DeprecatedPropTypes/DeprecatedViewPropTypes", path.resolve(rnPath, "Libraries/DeprecatedPropTypes/DeprecatedViewPropTypes")],
    ["../../vendor/document/selection/DocumentSelectionState", path.resolve(rnPath, "Libraries/vendor/document/selection/DocumentSelectionState")],
    ["../../Renderer/shims/NativeMethodsMixin", path.resolve(rnPath, "Libraries/Renderer/shims/NativeMethodsMixin")],
    ["../../Text/Text", path.resolve(rnPath, "Libraries/Text/Text")],
    ["../../Text/TextAncestor", path.resolve(rnPath, "Libraries/Text/TextAncestor")],
    ["../BatchedBridge/NativeModules", path.resolve(rnPath, "Libraries/BatchedBridge/NativeModules")],
    ["./TextInputState", path.resolve(rnPath, "Libraries/Components/TextInput/TextInputState")],
    ["../Touchable/TouchableWithoutFeedback", path.resolve(rnPath, "Libraries/Components/Touchable/TouchableWithoutFeedback")],
    ["./UIManagerProperties", path.resolve(rnPath, "Libraries/ReactNative/UIManagerProperties")],
    ["../Utilities/defineLazyObjectProperty", path.resolve(rnPath, "Libraries/Utilities/defineLazyObjectProperty")],
    ["../../ReactNative/UIManager", path.resolve(rnPath, "Libraries/ReactNative/UIManager")],
  ]);
}

function createRnTesterMap(rnPath, rndPath) {
  return new Map([
    // imports in RNTester app
    ["../../Libraries/Interaction/JSEventLoopWatchdog", path.resolve(rnPath, "Libraries/Interaction/JSEventLoopWatchdog")],
    ["../../Libraries/Utilities/infoLog", path.resolve(rnPath, "Libraries/Utilities/infoLog")],
    ["../../Libraries/Image/nativeImageSource", path.resolve(rnPath, "Libraries/Image/nativeImageSource")],
    ["../../Libraries/NewAppScreen", path.resolve(rnPath, "Libraries/NewAppScreen")],

  ]);
}


module.exports = {

  create: function(rnPath, rndPath) {

    let config = {
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

              let map;
              if(currentFile.includes(rnPath+'/')) {
                map = createRnToRndMap(rndPath);
              }
              else if(currentFile.includes("RNTester")) {
                map = createRnTesterMap(rnPath, rndPath)
              }
              else {
                map = createRndToRnMap(rnPath);
              }

              if (map.has(sourcePath))
                return map.get(sourcePath);

              return defaultResolvePath(sourcePath, currentFile, opts);
            },
          },
        ],
      ],
    };

    return config;


  }


};
