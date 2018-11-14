### Merging new react-native release step by step

- Create new local git branch based on latest master, for example, `merge/rn-0.57`
- Add original Facebook's react-native repo as remote to you git checkout:
  ```
  git remote add facebook https://github.com/facebook/react-native.git
  git fetch --all
  ``` 
- Determine the name of the source react-native branch to be merged. For example, `0.57-stable`
- Run script to do the merge and auto resolve merge conflicts "as theirs" for the files which were not modified by react-native-desktop. The maintaining list of files modified by desktop port is taken from `scripts/desktop-modified-files.txt`. This file should be constantly updated.
  ```
  scripts/react-native-fast-merge.sh facebook/0.57-stable
  ```
- Resolve merging conflicts in the remaining files and commit changes.
- Since git during merge doesn't delete files removed from the source branch (if the file had conflicts during the merge), it's required to take extra action to eliminate such files manually.
`diffoscope` cmd tool can be used to compare files availability in the diretories. You need to create separate clone of facebook's repository, checkout there merge source branch and do comparison with you working copy:
```
sudo apt-get install diffoscope
git clone https://github.com/facebook/react-native.git
cd react-native
git checkout 0.57-stable
diffoscope ./ ~/PATH_TO_WORKING_COPY/react-native-desktop/ --text result.txt
```
  Resulting `result.txt` file should start with files availability comparison between directories. It's simple to understand what new files were added to the react-native-desktop and manually remove files not related to react-native-desktop fork:
```
│  Libraries/Components/WebView/WebView.android.js
│ +Libraries/Components/WebView/WebView.desktop.js
│  Libraries/Components/WebView/WebView.ios.js
```
- Compile react-native-desktop native sources with CMake.
- Do `npm install` and make sure that `metro` package is patched correctly with `patch-package` tool (Reference https://www.npmjs.com/package/patch-package to update patch files if patching fails).
- Run metro packager with `npm start` to make sure that it starts correctly and without JS export collisions or any others warnings.
- Run JS RNTesterApp to verify that framework is starting and working fine in general.
- Run jest tests with `npm test` (on CI special cmd to start jest tests is used referenced in `package.json` as `test-ci`).
- Commit changes to remote and make sure that CI build is passing.
- Verify creation of new JS app based on your merge changes.
- Rebase all merged outside commits into single commit which, finally, will be merged in react-native-desktop master.

### Upgrading react-native version for status-react

- While status-react is not stick to specific remote branch of react-native-desktop, upgrading of main branch of status-react and react-native-desktop to new react-native-desktop version should happen at one point of time.
- Update `desktop_files/package.json` to reference upgraded react-native-desktop version with your working changes.
- Update `desktop_files/package.json` to depend on the same version of `react`, `react-dom` and range of `babel` packages. See https://github.com/status-im/status-react/commit/6cdff789e1859e2ad28d7d0a6aea536bf75fadbe for the reference.
- Update `patch-package` patches if required.
- Verify that dev build is working fine.
- Create PR and make sure that Jenkins build is passed fine.
