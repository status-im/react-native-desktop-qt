#ifndef REACTTESTCASE_H
#define REACTTESTCASE_H

#include <QObject>
#include <QTest>
#include <QTimer>
#include <QtQuick/QQuickView>
#include <functional>

class ReactView;
class ReactBridge;

class ReactTestCase : public QObject {
    Q_OBJECT
public:
    explicit ReactTestCase(QObject* parent = nullptr);
    virtual ~ReactTestCase() {}

signals:

protected:
    virtual void initTestCase();
    virtual void cleanupTestCase();
    void loadQML(const QUrl& qmlUrl);
    ReactView* rootView() const;
    ReactBridge* bridge();
    void showView();
    void waitAndVerifyBridgeReady();
    void waitAndVerifyJsAppStarted();
    void waitAndVerifyCondition(std::function<bool()> condition, const QString& timeoutMessage);

private:
    void registerReactQtTypes();

private:
    QQuickView* m_quickView = nullptr;
    QTimer timeoutTimer;
};

#endif // REACTTESTCASE_H
