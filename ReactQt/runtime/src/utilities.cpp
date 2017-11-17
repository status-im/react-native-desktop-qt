
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "utilities.h"

#include <QMetaType>
#include <QtQml>

#include "attachedproperties.h"
#include "componentmanagers/imagemanager.h"
#include "componentmanagers/viewmanager.h"
#include "layout/flexbox.h"
#include "reactitem.h"
#include "rootview.h"
#include "utilities.h"

const int MAJOR_VERSION = 0;
const int MINOR_VERSION = 1;

namespace utilities {

void registerReactTypes() {
    qmlRegisterUncreatableType<AttachedProperties>(
        "React", MAJOR_VERSION, MINOR_VERSION, "React", "React is not meant to be created directly");
    qmlRegisterType<ReactItem>("React", MAJOR_VERSION, MINOR_VERSION, "Item");
    qmlRegisterType<RootView>("React", MAJOR_VERSION, MINOR_VERSION, "RootView");
    qmlRegisterType<Flexbox>("React", MAJOR_VERSION, MINOR_VERSION, "Flexbox");
    qmlRegisterUncreatableType<ImageManager>("React",
                                             MAJOR_VERSION,
                                             MINOR_VERSION,
                                             "ReactImageManager",
                                             "ReactImageManager is not meant to be created directly");
    qmlRegisterUncreatableType<ViewManager>("React",
                                            MAJOR_VERSION,
                                            MINOR_VERSION,
                                            "ReactViewManager",
                                            "ReactViewManager is not meant to be created directly");
}

QString normalizeInputEventName(const QString& eventName) {
    QString tmp = eventName;
    if (eventName.startsWith("top"))
        return tmp;

    if (eventName.startsWith("on")) {
        tmp.replace(0, 2, "top");
    } else {
        tmp[0] = tmp[0].toUpper();
        tmp = "top" + tmp;
    }

    return tmp;
}

QQuickItem* createQMLItemFromSourceFile(QQmlEngine* qmlEngine, const QUrl& fileUrl) {
    QQmlComponent component(qmlEngine);
    component.loadUrl(fileUrl);
    if (!component.isReady()) {
        qCritical() << QString("Component for %1 is not loaded").arg(fileUrl.toString());
    }

    QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
    if (item == nullptr) {
        qCritical() << QString("Unable to construct item from component %1").arg(fileUrl.toString());
    }
    return item;
}
}
