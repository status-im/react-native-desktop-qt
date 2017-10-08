
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

#ifndef PIPEEXECUTOR_H
#define PIPEEXECUTOR_H

#include "executor.h"

class PipeExecutorPrivate;
class PipeExecutor : public Executor {
    Q_OBJECT
    Q_PROPERTY(QString nodePath READ nodePath WRITE setNodePath)
    Q_PROPERTY(QStringList arguments READ arguments WRITE setArguments)
    Q_PROPERTY(bool logErrors READ logErrors WRITE setLogErrors)

    Q_DECLARE_PRIVATE(PipeExecutor)

public:
    Q_INVOKABLE PipeExecutor(QObject* parent = 0);
    ~PipeExecutor();

    QString nodePath() const;
    void setNodePath(const QString& nodePath);

    QStringList arguments() const;
    void setArguments(const QStringList& arguments);

    bool logErrors() const;
    void setLogErrors(bool logErrors);

    void init() override;

    void injectJson(const QString& name, const QVariant& data) override;
    void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl) override;
    void executeJSCall(const QString& method,
                       const QVariantList& args = QVariantList(),
                       const ExecuteCallback& callback = ExecuteCallback()) override;

private:
    void processRequest(const QByteArray& request, const ExecuteCallback& callback = ExecuteCallback());

    QScopedPointer<PipeExecutorPrivate> d_ptr;
};

#endif // PIPEEXECUTOR_H
