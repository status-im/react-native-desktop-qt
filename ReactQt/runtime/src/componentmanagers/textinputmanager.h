
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef TEXTINPUTMANAGER_H
#define TEXTINPUTMANAGER_H

#include "viewmanager.h"

class PropertyHandler;
class TextInputManagerPrivate;
class TextInputManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(TextInputManager)

public:
    TextInputManager(QObject* parent = 0);
    virtual ~TextInputManager();

    virtual ViewManager* viewManager() override;
    virtual QString moduleName() override;

    virtual QStringList customDirectEventTypes() override;

public slots:
    void sendTextEditedToJs(QQuickItem* textInput);
    void sendSelectionChangeToJs(QQuickItem* textInput);
    void sendOnSubmitEditingToJs(QQuickItem* textInput);
    void sendOnEndEditingToJs(QQuickItem* textInput);
    void sendOnFocusToJs(QQuickItem* textInput);
    void sendOnKeyPressToJs(QQuickItem* textInput, QString keyText, QVariantList modifiers);
    void sendOnContentSizeChange(QQuickItem* textInput, double width, double height);
    bool onKeyPressed(QQuickItem* textInput,
                      QString keyText,
                      QVariantList modifiers,
                      QString submitKeyText,
                      QVariantList submitModifiers);

private:
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;
    virtual void configureView(QQuickItem* view) const override;

    void sendTextInputEvent(QQuickItem* textInput, QString eventName, QVariantMap additionalEventData = QVariantMap());

private:
    QScopedPointer<TextInputManagerPrivate> d_ptr;
};

#endif // TEXTINPUTMANAGER_H
