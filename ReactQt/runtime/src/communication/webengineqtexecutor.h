
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef WEBENGINEQTEXECUTOR_H
#define WEBENGINEQTEXECUTOR_H

#include <QByteArray>

#include "iexecutor.h"

class WebEngineQtExecutorPrivate;
class WebEngineQtExecutor : public IExecutor {
    Q_OBJECT
    Q_DECLARE_PRIVATE(WebEngineQtExecutor)

public:

    WebEngineQtExecutor(QObject* parent = nullptr);
    ~WebEngineQtExecutor();

    Q_INVOKABLE virtual void init();
    Q_INVOKABLE virtual void resetConnection();

    Q_INVOKABLE virtual void injectJson(const QString& name, const QVariant& data);
    Q_INVOKABLE virtual void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl);
    Q_INVOKABLE virtual void executeJSCall(const QString& method,
                                           const QVariantList& args = QVariantList(),
                                           const IExecutor::ExecuteCallback& callback = ExecuteCallback());

    Q_INVOKABLE void registerJSObject(const QString &id, QObject *object) override;

    Q_INVOKABLE void initJSconstraints() override;

private:
    QScopedPointer<WebEngineQtExecutorPrivate> d_ptr;
};

#endif // WEBENGINEQTEXECUTOR_H
