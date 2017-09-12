
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

#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include <QObject>
#include <QProcess>
#include <QQueue>
#include <QStateMachine>
#include <QString>
#include <QUrl>

#include <QDebug>

#include "reactpipeexecutor.h"

namespace {
QString default_executable{"node"};
QString default_script{"ubuntu-server.js"};
QStringList default_arguments{"--pipe"};

QString itemWithPath(const QString& item, const QStringList& searchPaths) {
    const QStringList local_paths{".", "bin"};
    for (const QString& searchPath : searchPaths) {
        for (const QString& localPath : local_paths) {
            QString testPath = searchPath + "/" + localPath + "/";
            if (QFileInfo::exists(testPath + item))
                return testPath + item;
        }
    }
    return item;
}

struct RegisterClass {
    RegisterClass() {
        qRegisterMetaType<ReactPipeExecutor*>();
    }
} registerClass;
}

class ReactPipeExecutorPrivate : public QObject {
    Q_OBJECT
public:
    bool logErrors = false;
    QProcess* nodeProcess = nullptr;
    QStateMachine* machina = nullptr;
    QByteArray inputBuffer;
    QQueue<QByteArray> requestQueue;
    QQueue<ReactExecutor::ExecuteCallback> responseQueue;

    void processRequests() {
        if (nodeProcess->state() != QProcess::Running || requestQueue.isEmpty()) {
            return;
        }

        QByteArray request = requestQueue.dequeue();
        quint32 length = request.size();
        nodeProcess->write((const char*)&length, sizeof(length));
        nodeProcess->write(request.constData(), request.size());
    }

public Q_SLOTS:
    void readReply() {
        if (inputBuffer.capacity() == 0) {
            quint32 length = 0;
            if (nodeProcess->bytesAvailable() < sizeof(length))
                return;
            nodeProcess->read((char*)&length, sizeof(length));
            inputBuffer.reserve(length);
        }

        inputBuffer += nodeProcess->read(inputBuffer.capacity() - inputBuffer.size());

        if (inputBuffer.size() < inputBuffer.capacity())
            return;

        ReactExecutor::ExecuteCallback callback = responseQueue.dequeue();
        if (callback) {
            QJsonDocument doc;
            if (inputBuffer != "undefined") {
                doc = QJsonDocument::fromJson(inputBuffer);
            }
            callback(doc);
        }

        inputBuffer.clear();
    }
};

ReactPipeExecutor::ReactPipeExecutor(QObject* parent) : ReactExecutor(parent), d_ptr(new ReactPipeExecutorPrivate) {
    Q_D(ReactPipeExecutor);

    // applicationDirPath needs to be called after creation of Application
    const QStringList search_paths{QDir::currentPath(), QCoreApplication::applicationDirPath()};

    d->nodeProcess = new QProcess(this);
    d->nodeProcess->setProgram(itemWithPath(default_executable, search_paths));
    d->nodeProcess->setArguments(QStringList() << itemWithPath(default_script, search_paths) << default_arguments);

    connect(d->nodeProcess, SIGNAL(readyReadStandardOutput()), d, SLOT(readReply()));
    connect(d->nodeProcess, &QProcess::readyReadStandardError, [=] {
        if (d->logErrors) {
            qWarning() << "Report from co-process: \"\"\"";
            qWarning().noquote() << d->nodeProcess->readAllStandardError().trimmed();
            qWarning() << "\"\"\"";
        }
    });

    d->machina = new QStateMachine(this);

    QState* initialState = new QState();
    QState* errorState = new QState();
    QState* readyState = new QState();

    initialState->addTransition(d->nodeProcess, SIGNAL(started()), readyState);
    readyState->addTransition(d->nodeProcess, SIGNAL(error(QProcess::ProcessError)), errorState);

    connect(initialState, &QAbstractState::entered, [=] { d->nodeProcess->start(); });
    connect(readyState, &QAbstractState::entered, [=] { d->processRequests(); });
    connect(errorState, &QAbstractState::entered, [=] { d->machina->stop(); });

    d->machina->addState(initialState);
    d->machina->addState(errorState);
    d->machina->addState(readyState);
    d->machina->setInitialState(initialState);
}

ReactPipeExecutor::~ReactPipeExecutor() {}

QString ReactPipeExecutor::nodePath() const {
    return d_func()->nodeProcess->program();
}

void ReactPipeExecutor::setNodePath(const QString& nodePath) {
    d_func()->nodeProcess->setProgram(nodePath);
}

QStringList ReactPipeExecutor::arguments() const {
    return d_func()->nodeProcess->arguments();
}

void ReactPipeExecutor::setArguments(const QStringList& arguments) {
    d_func()->nodeProcess->setArguments(arguments);
}

bool ReactPipeExecutor::logErrors() const {
    return d_func()->logErrors;
}

void ReactPipeExecutor::setLogErrors(bool logErrors) {
    d_func()->logErrors = logErrors;
}

void ReactPipeExecutor::init() {
    d_func()->machina->start();
}

void ReactPipeExecutor::injectJson(const QString& name, const QVariant& data) {
    Q_D(ReactPipeExecutor);

    QJsonDocument doc = QJsonDocument::fromVariant(data);

    processRequest(name.toLocal8Bit() + "=" + doc.toJson(QJsonDocument::Compact) + ";");
}

void ReactPipeExecutor::executeApplicationScript(const QByteArray& script, const QUrl& /*sourceUrl*/) {
    Q_D(ReactPipeExecutor);

    processRequest(script, [=](const QJsonDocument&) { Q_EMIT applicationScriptDone(); });
}

void ReactPipeExecutor::executeJSCall(const QString& method,
                                      const QVariantList& args,
                                      const ReactExecutor::ExecuteCallback& callback) {
    Q_D(ReactPipeExecutor);

    QByteArrayList stringifiedArgs;
    for (const QVariant& arg : args) {
        if (arg.type() == QVariant::List || arg.type() == QVariant::Map) {
            QJsonDocument doc = QJsonDocument::fromVariant(arg);
            stringifiedArgs += doc.toJson(QJsonDocument::Compact);
        } else {
            stringifiedArgs += '"' + arg.toString().toLocal8Bit() + '"';
        }
    }

    processRequest(QByteArray("__fbBatchedBridge.") + method.toLocal8Bit() + "(" + stringifiedArgs.join(',') + ");",
                   callback);
}

void ReactPipeExecutor::processRequest(const QByteArray& request, const ReactExecutor::ExecuteCallback& callback) {
    Q_D(ReactPipeExecutor);

    d->requestQueue.enqueue(request);
    d->responseQueue.enqueue(callback);
    d->processRequests();
}

#include "reactpipeexecutor.moc"
