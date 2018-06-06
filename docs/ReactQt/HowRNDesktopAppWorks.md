
# How react-native-desktop works internally

## Desktop project code
When you create project with `react-native init SomeProject` you have ios and android-specific files generated immediately. To add desktop platform files you have to invoke `react-native desktop` command (in your project folder). `SomeProject/desktop` folder will be generated. It contains `CMakeLists.txt` for building desktop Qt application.

## Running the app

### Participants
There are 3 participants in react-native-desktop application:
- Qt application
- Bundler
- JS server (right now called ubuntu-server, but this is subject to change)

`Qt application` - built from project in `desktop` folder. When runs it establishes communication with JS server and shows appropriate UI.

`Bundler` - this server runs on developer's machine and provides access to js files from project. Also it can generate "bundle" - single js file with all project code in it. Not needed for distribution. 

`JS server` - provides sandbox for running javascript project code. Communicates with Qt Application.

### Steps on app start

![Alt text](./react-native-desktop-workflow.svg)
<img src="./react-native-desktop-workflow">

1. When `react-native run-desktop` invoked, `Qt application` is built and launched. 
`desktop/main.cpp` is the one developer can change. Other sources can be found in `node_modules/react-native/ReactQt/application/src`. If you depend on any 3rd party react-native modules, their code also included at this point. 

2. `Qt application` connects to `Bundler` and receives single js file generated from all js files in a project.
This step is valid only for development environment. For production you should manually generate js bundle, mention it in `package.json` and it will be included in application resources.

3. `Qt application` connects to `JS server` and sends generated js code to it.
4. `JS server` invokes received javascript code in sandbox, receives JSON response and sends it back to `Qt application`
5. `Qt application` parses JSON response that contains intructions (what UI changes should be instantiated) and apply them. At this point user see native desktop Qt application with the UI he described in original JS files.


### Steps on app start


## Misc
Diagram code for future changes(mermaid format)

```
sequenceDiagram
QtApplication->>Bundler: Generate single js file from all js sources
Bundler-->>QtApplication: sends generated js file
QtApplication->>ubuntuserver.js: execute single js file in sandbox
ubuntuserver.js-->>ubuntuserver.js: executes js file
ubuntuserver.js->> QtApplication: send JSON response (result of code execution)
QtApplication-->> QtApplication: process JSON and instantiate UI components
```
