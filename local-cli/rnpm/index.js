"use strict";

const fs = require("fs");
const execSync = require("child_process").execSync;
const path = require("path");

// const semver = require("semver");
// const Registry = require("npm-registry");
// const validUrl = require("valid-url");

// const REACT_NATIVE_PACKAGE_JSON_PATH = function() {
//   return path.resolve(
//     process.cwd(),
//     "node_modules",
//     "react-native",
//     "package.json"
//   );
// };

const REACT_NATIVE_DESKTOP_GENERATE_PATH = function() {
  return path.resolve(
    process.cwd(),
    "node_modules",
    "react-native-desktop",
    "local-cli",
    "generate-desktop.js"
  );
};

// let npmConfReg = execSync("npm config get registry")
//   .toString()
//   .trim();

// let NPM_REGISTRY_URL = validUrl.is_uri(npmConfReg)
//   ? npmConfReg
//   : "http://registry.npmjs.org";

// const npm = new Registry({ registry: NPM_REGISTRY_URL });

// function getLatestVersion() {
//   return new Promise(function(resolve, reject) {
//     npm.packages.release("react-native-desktop", "latest", (err, releases) => {
//       if (err) {
//         reject(err);
//       } else if (!releases || releases.length === 0) {
//         reject(new Error("Could not find react-native-desktop@latest."));
//       } else {
//         resolve(releases[0].version);
//       }
//     });
//   });
// }

// function getMatchingVersion(version) {
//   console.log(`Checking for react-native-desktop version matching ${version}...`);
//   return new Promise(function(resolve, reject) {
//     npm.packages.range("react-native-desktop", version, (err, release) => {
//       if (err || !release) {
//         return getLatestVersion()
//           .then((latestVersion) => {
//             reject(
//               new Error(
//                 `Could not find react-native-desktop@${version}. ` +
//                   `Latest version of react-native-desktop is ${latestVersion}, try switching to ` +
//                   `react-native-desktop@${semver.major(
//                     latestVersion
//                   )}.${semver.minor(latestVersion)}.*.`
//               )
//             );
//           })
//           .catch((error) =>
//             reject(new Error(`Could not find react-native-desktop@${version}.`))
//           );
//       } else {
//         resolve(release.version);
//       }
//     });
//   });
// }

// const getAllReactNativeDesktopReleases = function(includeCanary) {
//   return new Promise((resolve, reject) => {
//     npm.packages.releases("react-native-desktop", (err, releases) => {
//       if (err) return reject(err);
//       resolve(
//         Object.keys(releases)
//           .filter((release) => {
//             if (["latest", "canary"].includes(release)) return false;
//             if (!includeCanary && release.includes("alpha")) return false;
//             return true;
//           }, {})
//           .map((r) => releases[r])
//       );
//     });
//   });
// };

// const getInstallPackage = function(version, includeCanary) {
//   if (version) {
//     let packageToInstall = "react-native-desktop";

//     const validVersion = semver.valid(version);
//     const validRange = semver.validRange(version);

//     if (validVersion || validRange) {
//       return getMatchingVersion(version);
//     } else {
//       return Promise.resolve(version);
//     }

//   } else {
//     const reactNativeVersion = getReactNativeVersion();

//     return getAllReactNativeDesktopReleases(includeCanary).then((releases) => {
//       releases.sort((a, b) => {
//         if (a.date < b.date) return 1;
//         if (a.date > b.date) return -1;
//         return 0;
//       });

//       for (const release of releases) {
//         if (
//           semver.satisfies(
//             reactNativeVersion,
//             release.peerDependencies["react-native"]
//           )
//         )
//           return release.version;
//       }

//       throw new Error(
//         `No version of 'react-native-desktop' found that satisfies a peer dependency on 'react-native@${reactNativeVersion}'`
//       );
//     });
//   }
// };

// const getReactNativeVersion = function() {
//   console.log("Reading react-native version from node_modules...");
//   if (fs.existsSync(REACT_NATIVE_PACKAGE_JSON_PATH())) {
//     const version = JSON.parse(
//       fs.readFileSync(REACT_NATIVE_PACKAGE_JSON_PATH(), "utf-8")
//     ).version;
//     return version;
//   }
// };

const getInstallPackage = function(version, includeCanary) {
  return Promise.resolve(`"https://github.com/status-im/react-native-desktop#master"`);
}

const getReactNativeAppName = function() {
  console.log("Reading application name from package.json...");
  return JSON.parse(fs.readFileSync("package.json", "utf8")).name;
};

/**
 * Check that 'react-native init' itself used yarn to install React Native.
 * When using an old global react-native-cli@1.0.0 (or older), we don't want
 * to install React Native with npm, and React + Jest with yarn.
 * Let's be safe and not mix yarn and npm in a single project.
 * @param projectDir e.g. /Users/martin/AwesomeApp
 */
const isGlobalCliUsingYarn = function(projectDir) {
  return fs.existsSync(path.join(projectDir, "yarn.lock"));
};

function desktop(config, args, options) {
  const packageToInstall = "react-native-desktop";

  const name = args[0] ? args[0] : getReactNativeAppName();
  const includeCanary = options.includeCanary ? options.includeCanary : false;
  const version = options.desktopVersion;

  getInstallPackage(version, includeCanary)
    .then((versionToInstall) => {
      const rnDesktopPackage = `${packageToInstall}@${versionToInstall}`;
      console.log(`Installing ${rnDesktopPackage}...`);
      const pkgmgr = isGlobalCliUsingYarn(process.cwd())
        ? "yarn add"
        : "npm install --save";

      execSync(`${pkgmgr} ${rnDesktopPackage}`, { stdio: "inherit" });
      console.log(`${rnDesktopPackage} succesfully installed`);

      const generateDesktop = require(REACT_NATIVE_DESKTOP_GENERATE_PATH());
      generateDesktop(process.cwd(), name);
    })
    .catch((error) => console.error(error.message));
};

module.exports = [{
  func: desktop,
  description: 'Generate React Native Desktop template project',
  name: 'desktop [name]',
  // options: [{
  //   command: '--desktopVersion [version]',
  //   description: 'The version of react-native-desktop to use.'
  // }]
}];
