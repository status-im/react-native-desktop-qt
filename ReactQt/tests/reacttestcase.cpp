#include "reacttestcase.h"

#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactitem.h"
#include "reactplugin.h"
#include "reactredboxitem.h"
#include "rootview.h"
#include "utilities.h"

const int TIMEOUT_INTERVAL = 30000;
const QString BUNDLE_URL = "http://localhost:8081/%1.bundle?platform=ubuntu&dev=true";

ReactTestCase::ReactTestCase(QObject* parent) : QObject(parent) {
    timeoutTimer.setSingleShot(true);
    timeoutTimer.setInterval(TIMEOUT_INTERVAL);
}

void ReactTestCase::initTestCase() {
    m_quickView = new QQuickView();
    utilities::registerReactTypes();
}

void ReactTestCase::cleanupTestCase() {
    if (m_quickView) {
        m_quickView->deleteLater();
        m_quickView = nullptr;
    }
}

void ReactTestCase::loadQML(const QUrl& qmlUrl) {
    m_quickView->setSource(qmlUrl);
    while (m_quickView->status() == QQuickView::Loading) {
        QCoreApplication::processEvents();
    }
}

void ReactTestCase::loadJSBundle(const QString& moduleName, const QString& bundlePath) {
    Q_ASSERT(!moduleName.isEmpty() && !bundlePath.isEmpty());

    RootView* reactView = rootView();
    Q_ASSERT(reactView);

    reactView->loadBundle(moduleName, BUNDLE_URL.arg(bundlePath));
}

RootView* ReactTestCase::rootView() const {
    Q_ASSERT(m_quickView);

    QObject* root = m_quickView->rootObject();
    Q_ASSERT(root);

    QQuickItem* rootView = root->findChild<QQuickItem*>("rootView");
    Q_ASSERT(rootView);

    RootView* reactView = qobject_cast<RootView*>(rootView);
    Q_ASSERT(reactView);

    return reactView;
}

ReactBridge* ReactTestCase::bridge() {
    ReactBridge* bridge = rootView()->bridge();
    Q_ASSERT(bridge);

    return bridge;
}

void ReactTestCase::showView() {
    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);
    m_quickView->show();
}

void ReactTestCase::waitAndVerifyBridgeReady() {
    waitAndVerifyCondition([=]() { return bridge()->ready(); }, "ReactBridge not ready, failed by timeout");
}

void ReactTestCase::waitAndVerifyJsAppStarted() {
    waitAndVerifyCondition([=]() { return bridge()->jsAppStarted(); }, "js app not started, failed by timeout");
}

void ReactTestCase::waitAndVerifyJSException(const QString& exceptionMessage) {
    waitAndVerifyCondition([=]() { return bridge()->redbox()->errorMessage() == exceptionMessage; },
                           QString("Expected JS exception \"%1\" was not received").arg(exceptionMessage));
}

void ReactTestCase::waitAndVerifyCondition(std::function<bool()> condition, const QString& timeoutMessage) {
    timeoutTimer.start();

    while (!condition() && timeoutTimer.isActive()) {
        QCoreApplication::processEvents();
    }
    QVERIFY2(timeoutTimer.isActive(), qUtf8Printable(timeoutMessage));
}
