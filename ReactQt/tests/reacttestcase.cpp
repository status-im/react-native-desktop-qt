#include "reacttestcase.h"

#include "attachedproperties.h"
#include "bridge.h"
#include "reactitem.h"
#include "reactplugin.h"
#include "redbox.h"
#include "rootview.h"
#include "utilities.h"

const int TIMEOUT_INTERVAL = 30000;
const QString BUNDLE_URL = "http://localhost:8081/%1.bundle?platform=desktop&dev=true";

ReactTestCase::ReactTestCase(QObject* parent) : QObject(parent) {
    timeoutTimer.setSingleShot(true);
    timeoutTimer.setInterval(TIMEOUT_INTERVAL);
}

void ReactTestCase::initTestCase() {
    m_quickView = new QQuickView();
    utilities::registerReactTypes();

    clickDelayTimer = new QTimer(this);
    clickDelayTimer->setSingleShot(true);
    clickDelayTimer->setInterval(CLICK_TIMER_DELAY);

    initDelayTimer = new QTimer(this);
    initDelayTimer->setSingleShot(true);
    initDelayTimer->setInterval(INIT_TIMER_DELAY);
}

void ReactTestCase::cleanupTestCase() {
    if (m_quickView) {
        m_quickView->deleteLater();
        m_quickView = nullptr;
    }
}

void ReactTestCase::init() {
    initDelayTimer->start();
    waitAndVerifyCondition([=]() { return !initDelayTimer->isActive(); }, "Timer timeout was not triggered");
}

void ReactTestCase::cleanup() {
    bridge()->reset();
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

QQuickItem* ReactTestCase::topJSComponent() const {
    // Even when in JS we have only one <Image> component returned in render(),
    // it is wrapped in <View> component implicitly, so we have hierarchy in QML:
    // ReactView
    //  |-<View>
    //    |-<Image>

    QList<QQuickItem*> reactViewChilds = rootView()->childItems();
    Q_ASSERT(reactViewChilds.count() == 1);

    QQuickItem* view = reactViewChilds[0];
    QList<QQuickItem*> viewChilds = view->childItems();
    Q_ASSERT(viewChilds.count() == 1);

    QQuickItem* control = viewChilds[0];
    Q_ASSERT(control);

    return control;
}

QVariant ReactTestCase::valueOfControlProperty(QQuickItem* control, const QString& propertyName) {
    Q_ASSERT(control);
    return control->property(propertyName.toStdString().c_str());
}

Bridge* ReactTestCase::bridge() {
    Bridge* bridge = rootView()->bridge();
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
    waitAndVerifyCondition([=]() { return bridge()->redbox()->errorMessage().startsWith(exceptionMessage); },
                           QString("Expected JS exception \"%1\" was not received").arg(exceptionMessage));
}

void ReactTestCase::waitAndVerifyCondition(std::function<bool()> condition, const QString& timeoutMessage) {
    timeoutTimer.start();

    while (!condition() && timeoutTimer.isActive()) {
        QCoreApplication::processEvents();
    }
    QVERIFY2(timeoutTimer.isActive(), qUtf8Printable(timeoutMessage));
}

void ReactTestCase::clickItem(QQuickItem* item) {
    auto point = item->mapToScene(QPoint(0, 0)).toPoint();
    point.rx() += item->width() / 2;
    point.ry() += item->height() / 2;
    QTest::mouseClick(m_quickView, Qt::LeftButton, Qt::NoModifier, point);
}
