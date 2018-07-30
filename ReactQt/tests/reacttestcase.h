#ifndef REACTTESTCASE_H
#define REACTTESTCASE_H

#include <QObject>
#include <QTest>
#include <QTimer>
#include <QtQuick/QQuickView>
#include <functional>

class RootView;
class Bridge;

#define INIT_TEST_CASE_DEFAULT(BaseClass)                                                                              \
    virtual void initTestCase() override {                                                                             \
        BaseClass::initTestCase();                                                                                     \
    }

#define CLEANUP_TEST_CASE_DEFAULT(BaseClass)                                                                           \
    virtual void cleanupTestCase() override {                                                                          \
        BaseClass::cleanupTestCase();                                                                                  \
    }

#define INIT_DEFAULT(BaseClass)                                                                                        \
    virtual void init() override {                                                                                     \
        BaseClass::init();                                                                                             \
    }

#define CLEANUP_DEFAULT(BaseClass)                                                                                     \
    virtual void cleanup() override {                                                                                  \
        BaseClass::cleanup();                                                                                          \
    }

class ReactTestCase : public QObject {
    Q_OBJECT
public:
    explicit ReactTestCase(QObject* parent = nullptr);
    virtual ~ReactTestCase() {}

    virtual void initTestCase();
    virtual void cleanupTestCase();

signals:

protected:
    void loadQML(const QUrl& qmlUrl);
    RootView* rootView() const;
    void loadJSBundle(const QString& moduleName, const QString& bundlePath);
    Bridge* bridge();
    void showView();
    void waitAndVerifyBridgeReady();
    void waitAndVerifyJsAppStarted();
    void waitAndVerifyJSException(const QString& exceptionMessage);
    void waitAndVerifyCondition(std::function<bool()> condition, const QString& timeoutMessage);
    QQuickItem* topJSComponent() const;
    QVariant valueOfControlProperty(QQuickItem* control, const QString& propertyName);
    void clickItem(QQuickItem* item);
    virtual void init();
    virtual void cleanup();

private:
    void registerReactQtTypes();

private:
    QQuickView* m_quickView = nullptr;
    QTimer timeoutTimer;

protected:
    QTimer* clickDelayTimer = nullptr;
    QTimer* initDelayTimer = nullptr;

    const int CLICK_TIMER_DELAY = 1000;
    const int INIT_TIMER_DELAY = 4000;
};

#endif // REACTTESTCASE_H
