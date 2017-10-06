
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

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <functional>

#include <QObject>
#include <QVariant>

class Executor : public QObject {
    Q_OBJECT

public:
    typedef std::function<void(const QJsonDocument&)> ExecuteCallback;

    Executor(QObject* parent);
    ~Executor();

    virtual void init() = 0;

    virtual void injectJson(const QString& name, const QVariant& data) = 0;
    virtual void executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl) = 0;
    virtual void executeJSCall(const QString& method,
                               const QVariantList& args = QVariantList(),
                               const ExecuteCallback& callback = ExecuteCallback()) = 0;

Q_SIGNALS:
    // TODO: KOZIEIEV: remove from Executor. Maybe in Bridge. Executor shouldn't know about app-related things.
    void applicationScriptDone();
};

#endif // EXECUTOR_H
