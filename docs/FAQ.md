### How do Qt and Node communicate?

Qt app contains one type of communication - it sends js code and receives JSON response to process.

When started it sends full JS code of an application.

When running it sends short pieces of JS code to invoke React Native functions and/or notify about events.

### How would you best describe the role of the `react-native-desktop` repo?

Also: what code should live there? A build tool? Housing build related code?

Right now `react-native-desktop` repo contains original `react-native` code plus desktop support, so its role is `Substitution of react-native repo with desktop platform added`. 
I hope that soon we will cleanup repo and will leave only desktop-related code in it (no original react native code). Than role of repo will be `extension of react-native for desktop support`.

In both cases this repo should contain code and tools required to create, build and deploy react-native applications for desktop.

### What repo was `react-native-desktop` originally forked from?

It was forked from Canonical/react-native-linux. Canonical in its turn forked facebook/react-native. Right now Canonical removed their fork, so we don’t have it mentioned in github.

### Does the bundler step in the diagram only happen once at app startup or is all of the JS code appended to a single file on every reload?

On every reload Qt app requests single js files and sends it to execution in ubuntu-server. It is like restart, but quicker. Bundler caches that js file, so collecting it together doesn’t take much time.

### Is there a dedicated node mule within react-native for communicating with qt at the javascript code level or does that happen elsewhere, say, in the build pipeline?

No. React-native is designed to have own js part and communication with native part.
Qt code in react-native-desktop contains a bunch of modules that implement controls and API’s natively. 
When Qt Application starts it sends `config` - information about what modules it implements on native side - their names, ids, lists of functions, constants, etc. From this moment react-native part of js code can “invoke” that native methods just by mentioning them in JSON results.

Here is an example communication between Qt app and ubuntu-server. In the app there is a button that changes color when pressed.

When user clicks button, we send following code line for execution in Js:
`__fbBatchedBridge.callFunctionReturnFlushedQueue(\"RCTEventEmitter\",\"receiveEvent\",[2,\"topPress\",{}]);`
2 - is an id of specific button component

Here is a JSON result that we receive from function above:

```json
[
    [
        27
    ],
    [
        2
    ],
    [
        [
            2,
            "RCTButtonView",
            {
                "color": "green"
            }
        ]
    ],
    6
]
```

When it is processed on Qt side, it means that function 2 from Module 27 should be invoked with passed params. (react-native knows module ids from config).

### Where can I read some intro on React Native internals?

https://www.logicroom.co/react-native-architecture-explained/, https://facebook.github.io/react-native/docs/communication-ios.html.

### How do I control Qt logging?

The app leverages [QLoggingCategory](http://doc.qt.io/qt-5/qloggingcategory.html#configuring-categories) to allow filtering of categories. If the app sets the default categories using [QLoggingCategory::setFilterRules](http://doc.qt.io/qt-5/qloggingcategory.html#setFilterRules), then the categories can only be overriden using the `QT_LOGGING_CONF` and `QT_LOGGING_RULES` environment variables.

Order of evaluation:

1. `[QLibraryInfo::DataPath]/qtlogging.ini`
1. `QtProject/qtlogging.ini`
1. `setFilterRules()`
1. `QT_LOGGING_CONF`
1. `QT_LOGGING_RULES`

Existing categories:

- `ViewManager`
- `Flexbox`
- `Networking`
- `UIManager`
- `WebSocketModule`

Example:

```shell
export QT_LOGGING_RULES="VIEWMANAGER=false;NETWORKING=true"
```
