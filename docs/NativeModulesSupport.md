## 3rd party react-native-desktop modules support

## Modules without native implementation (pure JS)
Pure JavaScript react-native modules can be used with react-native-desktop without limitations.  

## Modules with native implementation

Check [List of supported or partially supported native modules](docs/SupportedNativeModulesList.md), maybe the module you need already ported for desktop. If not, creation of custom react-native `native` modules or `adopting of available native modules` described below:

### Implementing 3rd party modules
react-native-desktop provides support for 3rd party native modules to extend functionality of original framework.

For example, lets define custom native module `Status` which can be finally referenced from JavaScript source code.

Define custom C++ class to declare custom native module `Status`. It declares function `initJail` accessible from JavaScript which returns back promise callback to JS.

content of `rctstatus.h`
```
#ifndef RCTSTATUS_H
#define RCTSTATUS_H

#include "moduleinterface.h"

#include <QVariantMap>

class RCTStatusPrivate;
class RCTStatus : public QObject, public ModuleInterface {
    Q_OBJECT

    Q_DECLARE_PRIVATE(RCTStatus)

public:
    Q_INVOKABLE RCTStatus(QObject* parent = 0);
    ~RCTStatus();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;
    QList<ModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;

    Q_INVOKABLE void initJail(QString js, double callbackId);
private:
    QScopedPointer<RCTStatusPrivate> d_ptr;
};

#endif // RCTSTATUS_H
```

content of `rctstatus.cpp`

```
#include "rctstatus.h"

namespace {
struct RegisterQMLMetaType {
    RegisterQMLMetaType() {
        // Make RCTStatus type available for Qt Meta-Object System
        qRegisterMetaType<RCTStatus*>();
    }
} registerMetaType;
} // namespace

class RCTStatusPrivate {
};

RCTStatus::RCTStatus(QObject* parent) : QObject(parent), d_ptr(new RCTStatusPrivate) {
}

RCTStatus::~RCTStatus() {}

void RCTStatus::setBridge(Bridge* bridge) {
    Q_D(RCTStatus);
    d->bridge = bridge;
}

QString RCTStatus::moduleName() {
    // Native module name accessible from JavaScript runtime
    return "Status";
}

QList<ModuleMethod*> RCTStatus::methodsToExport() {
    return QList<ModuleMethod*>{};
}

QVariantMap RCTStatus::constantsToExport() {
    return QVariantMap();

void RCTStatus::initJail(QString js, double callbackId) {
    Q_D(RCTStatus);

    // Some work happens here

    // Invoke callback to JavaScript
    d->bridge->invokePromiseCallback(callbackId, QVariantList{ "{\"result\":\"\"}" });
}
}
```

Integrate new created C++ sources into `cmake` build script by defining new `CMakeList.txt` file in the same directory.

content of `CMakeList.txt`:

```
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_TYPE_NAMES ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_TYPE_NAMES}
                                                     \"RCTStatus\" PARENT_SCOPE)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_SRC}
                                              ${CMAKE_CURRENT_SOURCE_DIR}/rctstatus.cpp PARENT_SCOPE)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_LIBS ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_LIBS}
                                                "-framework Foundation"
                                                pthread PARENT_SCOPE)

set(REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_INCLUDE_DIRS ${REACT_NATIVE_DESKTOP_EXTERNAL_MODULES_INCLUDE_DIRS}
                                               "/usr/status-im/status-go/include" PARENT_SCOPE)
```

`package.json` file of your root project should be extended with relative path to the new created native module (directory where `CMakeList.txt` resides):

```
"desktopExternalModules": [
    "modules/react-native-status/desktop"
  ]
```

Next run of `react-native run-desktop` command should pick up new added `Status` native module.
