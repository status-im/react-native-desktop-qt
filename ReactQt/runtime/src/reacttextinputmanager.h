
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef REACTTEXTINPUTMANAGER_H
#define REACTTEXTINPUTMANAGER_H

#include "reactviewmanager.h"

class ReactPropertyHandler;
class ReactTextInputManagerPrivate;
class ReactTextInputManager : public ReactViewManager {
    Q_OBJECT
    Q_INTERFACES(ReactModuleInterface)
    Q_DECLARE_PRIVATE(ReactTextInputManager)

public:
    ReactTextInputManager(QObject* parent = 0);
    virtual ~ReactTextInputManager();

    virtual ReactViewManager* viewManager() override;
    virtual QString moduleName() override;

    virtual QStringList customDirectEventTypes() override;

public slots:
    void sendTextEditedToJs(QQuickItem* textInput);
    void sendSelectionChangeToJs(QQuickItem* textInput);

private:
    virtual QString qmlComponentFile() const override;
    virtual void configureView(QQuickItem* view) const override;

private:
    QScopedPointer<ReactTextInputManagerPrivate> d_ptr;
};

#endif // REACTTEXTINPUTMANAGER_H
