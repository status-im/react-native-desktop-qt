
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifdef JAVASCRIPTCORE_ENABLED

#include "javascriptcoreexecutor.h"

#include "bridge.h"
#include "jscutilities.h"

#include "cxxreact/Instance.h"
#include "cxxreact/JSBigString.h"
#include <cxxreact/MessageQueueThread.h>

#include <QDebug>
#include <QThread>
#include <QTimer>

using namespace facebook::react;

typedef std::function<void(QString error)> RCTJavaScriptCompleteBlock;
typedef std::function<void(int result, QString error)> RCTJavaScriptCallback;

namespace facebook {
namespace react {

class RCTMessageThread : public QObject, public MessageQueueThread {
    Q_OBJECT
public:
    RCTMessageThread(RCTJavaScriptCompleteBlock errorBlock);
    ~RCTMessageThread() override;
    void runOnQueue(std::function<void()>&&) override;
    void runOnQueueSync(std::function<void()>&&) override;
    void quitSynchronous() override;
    void setRunLoop();

Q_SIGNALS:
    void sheduleRun(std::function<void()>);

private Q_SLOTS:
    void execFunc(std::function<void()>);

private:
    void tryFunc(const std::function<void()>& func);
    void runAsync(std::function<void()> func);
    void runSync(std::function<void()> func);

    std::atomic_bool m_shutdown;
    QThread m_thread;
};

RCTMessageThread::RCTMessageThread(RCTJavaScriptCompleteBlock errorBlock) : m_shutdown(false) {
    qRegisterMetaType<std::function<void()>>("std::function<void()>");
    this->moveToThread(&m_thread);

    connect(this, &RCTMessageThread::sheduleRun, this, &RCTMessageThread::execFunc, Qt::QueuedConnection);
    m_thread.start();
}

RCTMessageThread::~RCTMessageThread() {}

// This is analogous to dispatch_async
void RCTMessageThread::runAsync(std::function<void()> func) {}

// This is analogous to dispatch_sync
void RCTMessageThread::runSync(std::function<void()> func) {}

void RCTMessageThread::tryFunc(const std::function<void()>& func) {}

void RCTMessageThread::execFunc(std::function<void()> func) {
    func();
}

void RCTMessageThread::runOnQueue(std::function<void()>&& func) {
    if (m_shutdown) {
        return;
    }

    emit sheduleRun(func);
}

void RCTMessageThread::runOnQueueSync(std::function<void()>&& func) {
    if (m_shutdown) {
        return;
    }

    func();
}

void RCTMessageThread::quitSynchronous() {
    m_shutdown = true;
}

void RCTMessageThread::setRunLoop() {}
}
}

struct RCTInstanceCallback : public InstanceCallback {
    Bridge* bridge_;
    RCTInstanceCallback(Bridge* bridge) : bridge_(bridge) {}
    void onBatchComplete() override {
        // There's no interface to call this per partial batch
        bridge_->partialBatchDidFlush();
        bridge_->batchDidComplete();
    }
};

class JavaScriptCoreExecutorPrivate : public QObject {
    Q_OBJECT
public:
    JavaScriptCoreExecutorPrivate(JavaScriptCoreExecutor* e) {}

public:
    std::shared_ptr<RCTMessageThread> _jsMessageThread;
    std::shared_ptr<Instance> _reactInstance;
    Bridge* bridge = nullptr;
};

JavaScriptCoreExecutor::JavaScriptCoreExecutor(QObject* parent)
    : IExecutor(parent), d_ptr(new JavaScriptCoreExecutorPrivate(this)) {
    Q_D(JavaScriptCoreExecutor);
}

JavaScriptCoreExecutor::~JavaScriptCoreExecutor() {}

void JavaScriptCoreExecutor::injectJson(const QString& name, const QVariant& data) {}

void JavaScriptCoreExecutor::executeApplicationScript(const QByteArray& script, const QUrl& sourceUrl) {
    Q_D(JavaScriptCoreExecutor);
    d->_jsMessageThread->runOnQueue([=] {
        d->_reactInstance->loadScriptFromString(
            std::make_unique<JSBigStdString>(script.toStdString()), sourceUrl.toString().toStdString(), true);
        d->bridge->setReady(true);
    });
}

void JavaScriptCoreExecutor::executeJSCall(const QString& method,
                                           const QVariantList& args,
                                           const IExecutor::ExecuteCallback& callback) {
    // Q_ASSERT(args.size() == 3);
    if (args.size() != 3)
        return;
    d_func()->_reactInstance->callJSFunction(args.at(0).toString().toStdString(),
                                             args.at(1).toString().toStdString(),
                                             utilities::qvariantToDynamic(args.at(2).toList()));
}

void* JavaScriptCoreExecutor::getJavaScriptContext() {
    Q_D(JavaScriptCoreExecutor);
    if (d->_reactInstance.get()) {
        return d->_reactInstance->getJavaScriptContext();
    } else {
        return nullptr;
    }
}

void JavaScriptCoreExecutor::executeOnJavaScriptThread(std::function<void()> func) {
    Q_D(JavaScriptCoreExecutor);
    d->_jsMessageThread->runOnQueue(std::function<void()>(func));
}

void JavaScriptCoreExecutor::setBridge(Bridge* bridge) {
    Q_D(JavaScriptCoreExecutor);
    d->bridge = bridge;
}

void JavaScriptCoreExecutor::init() {
    Q_D(JavaScriptCoreExecutor);
    Q_ASSERT(d->bridge);

    std::shared_ptr<JSExecutorFactory> executorFactory;
    executorFactory.reset(new JSCExecutorFactory(folly::dynamic::object("OwnerIdentity", "ReactNative")(
        "AppIdentity", "ReactNativeApp")("DeviceIdentity", "unknown")("UseCustomJSC", false)));

    d->_jsMessageThread = std::make_shared<RCTMessageThread>([=](QString error) {
        if (error.isEmpty()) {
        }
    });

    std::shared_ptr<ModuleRegistry> moduleRegistry;
    moduleRegistry.reset(new ModuleRegistry(std::vector<std::unique_ptr<NativeModule>>(), [=](const std::string& name) {
        qDebug() << "Native module \"" << name.c_str() << "\" not found !";
        return false;
    }));

    d->_reactInstance.reset(new Instance);

    d->bridge->initModules();
    moduleRegistry->registerModules(d->bridge->modules());

    d->_jsMessageThread->runOnQueue([=] {
        d->_reactInstance->initializeBridge(
            std::make_unique<RCTInstanceCallback>(d->bridge), executorFactory, d->_jsMessageThread, moduleRegistry);
    });

    QTimer::singleShot(500, [=]() { d->bridge->loadSource(); });
}

#include "javascriptcoreexecutor.moc"

#endif // JAVASCRIPTCORE_ENABLED
