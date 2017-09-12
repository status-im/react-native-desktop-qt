#include "reacttestcase.h"

#include "reactattachedproperties.h"
#include "reactbridge.h"
#include "reactflexlayout.h"
#include "reactitem.h"
#include "reactview.h"

const int TIMEOUT_INTERVAL = 30000;

ReactTestCase::ReactTestCase(QObject* parent) : QObject(parent) {
    timeoutTimer.setSingleShot(true);
    timeoutTimer.setInterval(TIMEOUT_INTERVAL);
}

void ReactTestCase::initTestCase() {
    m_quickView = new QQuickView();
    registerReactQtTypes();
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

ReactView* ReactTestCase::rootView() const {
    Q_ASSERT(m_quickView);

    QObject* root = m_quickView->rootObject();
    Q_ASSERT(root);

    QQuickItem* rootView = root->findChild<QQuickItem*>("rootView");
    Q_ASSERT(rootView);

    ReactView* reactView = qobject_cast<ReactView*>(rootView);
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

void ReactTestCase::waitAndVerifyCondition(std::function<bool()> condition, const QString& timeoutMessage) {
    timeoutTimer.start();

    while (!condition() && timeoutTimer.isActive()) {
        QCoreApplication::processEvents();
    }
    QVERIFY2(timeoutTimer.isActive(), qUtf8Printable(timeoutMessage));
}

void ReactTestCase::registerReactQtTypes() {
    qmlRegisterUncreatableType<ReactAttachedProperties>(
        "React", 0, 1, "React", "React is not meant to be created directly");
    qmlRegisterUncreatableType<ReactFlexLayout>("React", 0, 1, "Flex", "Flex is not meant to be created directly");
    qmlRegisterType<ReactItem>("React", 0, 1, "Item");
    qmlRegisterType<ReactView>("React", 0, 1, "RootView");
}
