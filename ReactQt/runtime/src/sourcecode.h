
/**
 * Copyright (C) 2016, Canonical Ltd.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 * Author: Justin McPherson <justin.mcpherson@canonical.com>
 *
 */

#ifndef SOURCECODE_H
#define SOURCECODE_H

#include <QByteArray>
#include <QUrl>

#include "moduleinterface.h"

class QNetworkAccessManager;

class SourceCodePrivate;
class SourceCode : public QObject, public ModuleInterface {
    Q_OBJECT
    // Q_PLUGIN_METADATA(IID ReactModuleInterface_IID)
    Q_INTERFACES(ModuleInterface)

    Q_PROPERTY(QUrl scriptURL READ scriptUrl CONSTANT)
    Q_PROPERTY(QByteArray sourceCode READ sourceCode NOTIFY sourceCodeChanged SCRIPTABLE false)
    Q_PROPERTY(int retryCount READ retryCount WRITE setRetryCount NOTIFY retryCountChanged SCRIPTABLE false)

    Q_INVOKABLE REACT_PROMISE void getScriptText(const ModuleInterface::ListArgumentBlock& resolve,
                                                 const ModuleInterface::ListArgumentBlock& reject);

    Q_DECLARE_PRIVATE(SourceCode);

public:
    SourceCode(QObject* parent = 0);
    ~SourceCode();

    void setBridge(Bridge* bridge) override;

    // TODO: this doesnt seem right
    ViewManager* viewManager() override;

    QString moduleName() override;

    QUrl scriptUrl() const;
    void setScriptUrl(const QUrl& source);

    QByteArray sourceCode() const;

    int retryCount() const;
    void setRetryCount(int retryCount);

    void loadSource(QNetworkAccessManager* nam);

Q_SIGNALS:
    void scriptUrlChanged();
    void sourceCodeChanged();
    void retryCountChanged();
    void loadFailed();

private:
    QScopedPointer<SourceCodePrivate> d_ptr;
};

#endif // SOURCECODE_H
