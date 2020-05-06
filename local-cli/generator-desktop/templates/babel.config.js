const defaultResolvePath = require('babel-plugin-module-resolver').resolvePath;

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
          let map = new Map([
            ["../Utilities/Platform",     "../../../react-native-desktop-qt/Libraries/Utilities/Platform"],
            ["./Image",                   "../../../react-native-desktop-qt/Libraries/Image/Image"],
            ["../../Utilities/Platform",  "../../../../react-native-desktop-qt/Libraries/Utilities/Platform"]
          ]);
          if(map.has(sourcePath))
            return map.get(sourcePath);

          return defaultResolvePath(sourcePath, currentFile, opts);
        },
      },
    ],
  ],
};
