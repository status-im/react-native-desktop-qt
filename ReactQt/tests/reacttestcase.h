#ifndef REACTTESTCASE_H
#define REACTTESTCASE_H

#include <QObject>
#include <QTest>
#include <QTimer>
#include <QtQuick/QQuickView>
#include <functional>

class RootView;
class ReactBridge;

#define INIT_TEST_CASE_DEFAULT(BaseClass)                                                                              \
    virtual void initTestCase() override {                                                                             \
        BaseClass::initTestCase();                                                                                     \
    }

#define CLEANUP_TEST_CASE_DEFAULT(BaseClass)                                                                           \
    virtual void cleanupTestCase() override {                                                                          \
        BaseClass::cleanupTestCase();                                                                                  \
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
    ReactBridge* bridge();
    void showView();
    void waitAndVerifyBridgeReady();
    void waitAndVerifyJsAppStarted();
    void waitAndVerifyJSException(const QString& exceptionMessage);
    void waitAndVerifyCondition(std::function<bool()> condition, const QString& timeoutMessage);

private:
    void registerReactQtTypes();

private:
    QQuickView* m_quickView = nullptr;
    QTimer timeoutTimer;
};

#endif // REACTTESTCASE_H
