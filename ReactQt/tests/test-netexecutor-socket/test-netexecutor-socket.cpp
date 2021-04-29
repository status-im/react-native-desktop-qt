/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QScopedPointer>
#include <QSignalSpy>
#include <QTest>
#include <QVariant>
#include <QtMath>
#include <QtQuick/QQuickView>

#include "bridge.h"
#include "communication/nodejsexecutor.h"
#include "communication/serverconnection.h"
#include "reacttestcase.h"
#include <QTcpServer>

const int TEST_EXECUTOR_PORT = 3000;
const int MAX_PACKAGES_COUNT = 9;
const int CHUNK_SIZE = 100000;

class TestNetExecutorSocket : public ReactTestCase {
    Q_OBJECT

protected:
    void sendPackageInTwoWrites(quint32 bytesCount);
    void sendPackageInOneWrite(quint32 bytesCount);
    void sendPackageInChunks(quint32 bytesCount);
    void sendTwoPackagesInOneWrite(quint32 package1bytesCount, quint32 package2bytesCount);
    QByteArray packageData(int bytesCount);
    QByteArray packageHeader(quint32 bytesCount);
    QByteArray package(quint32 bytesCount);
    quint32 packageSize(quint32 packageNumber);
    void waitAndCheckPackagesReceived(QSignalSpy& spy, int count);

private slots:
    void initTestCase() override;

    void testSendingPackagesInTwoWrites();
    void testSendingPackagesInOneWrite();
    void testSendingPackagesInChunks();
    void testSendingTwoPackagesAtOnce();

private:
    QSharedPointer<NodeJsExecutor> m_executor;
    QSharedPointer<QTcpServer> m_server;
    QSharedPointer<QTcpSocket> m_socket;
};

void TestNetExecutorSocket::initTestCase() {

    m_server = QSharedPointer<QTcpServer>(new QTcpServer());
    m_server->listen(QHostAddress::LocalHost, TEST_EXECUTOR_PORT);

    RemoteServerConnection* connection = new RemoteServerConnection;
    connection->setPort(TEST_EXECUTOR_PORT);

    m_executor = QSharedPointer<NodeJsExecutor>(new NodeJsExecutor(connection));
    m_executor->init();

    waitAndVerifyCondition([=]() { return m_server->hasPendingConnections(); },
                           "NetExecutor didn't connect to Tcp server");
    m_socket = QSharedPointer<QTcpSocket>(m_server->nextPendingConnection());
    QVERIFY(m_socket->isValid());
}

void TestNetExecutorSocket::sendPackageInTwoWrites(quint32 bytesCount) {
    m_socket->write(packageHeader(bytesCount));
    m_socket->write(packageData(bytesCount));
    m_socket->flush();
}

void TestNetExecutorSocket::sendPackageInOneWrite(quint32 bytesCount) {
    m_socket->write(package(bytesCount));
    m_socket->flush();
}

void TestNetExecutorSocket::sendPackageInChunks(quint32 bytesCount) {
    QByteArray ba = package(bytesCount);

    while (ba.length()) {
        QByteArray chunk = ba.left(CHUNK_SIZE);
        ba.remove(0, chunk.length());
        m_socket->write(chunk);
        m_socket->flush();
    }
}

void TestNetExecutorSocket::sendTwoPackagesInOneWrite(quint32 package1bytesCount, quint32 package2bytesCount) {
    QByteArray data;
    data += package(package1bytesCount);
    data += package(package2bytesCount);

    m_socket->write(data);
    m_socket->flush();
}

QByteArray TestNetExecutorSocket::packageData(int bytesCount) {
    return QByteArray(bytesCount, 'x');
}

QByteArray TestNetExecutorSocket::packageHeader(quint32 bytesCount) {
    return QByteArray((const char*)&bytesCount, sizeof(bytesCount));
}

QByteArray TestNetExecutorSocket::package(quint32 bytesCount) {
    return packageHeader(bytesCount) + packageData(bytesCount);
}

quint32 TestNetExecutorSocket::packageSize(quint32 packageNumber) {
    return qPow(10, packageNumber);
}

void TestNetExecutorSocket::waitAndCheckPackagesReceived(QSignalSpy& spy, int count) {
    waitAndVerifyCondition([&]() { return spy.count() == count; }, "NetExecutor didn't get all the data");

    for (int i = 0; i < count; ++i) {
        int bytesSent = packageSize(i);
        int bytesReceived = spy.takeFirst().at(0).toInt();
        QCOMPARE(bytesSent, bytesReceived);
    }
}

void TestNetExecutorSocket::testSendingPackagesInTwoWrites() {

    QSignalSpy spy(m_executor.data(), &NodeJsExecutor::commandReceived);

    for (int i = 0; i < MAX_PACKAGES_COUNT; ++i) {
        quint32 bytesCount = packageSize(i);
        sendPackageInTwoWrites(bytesCount);
    }

    waitAndCheckPackagesReceived(spy, MAX_PACKAGES_COUNT);
}

void TestNetExecutorSocket::testSendingPackagesInOneWrite() {
    QSignalSpy spy(m_executor.data(), &NodeJsExecutor::commandReceived);

    for (int i = 0; i < MAX_PACKAGES_COUNT; ++i) {
        quint32 bytesCount = packageSize(i);
        sendPackageInOneWrite(bytesCount);
    }

    waitAndCheckPackagesReceived(spy, MAX_PACKAGES_COUNT);
}

void TestNetExecutorSocket::testSendingPackagesInChunks() {
    QSignalSpy spy(m_executor.data(), &NodeJsExecutor::commandReceived);

    for (int i = 0; i < MAX_PACKAGES_COUNT; ++i) {
        quint32 bytesCount = packageSize(i);
        sendPackageInChunks(bytesCount);
    }

    waitAndCheckPackagesReceived(spy, MAX_PACKAGES_COUNT);
}

void TestNetExecutorSocket::testSendingTwoPackagesAtOnce() {

    // This test covers previously existing bug. NetExecutor was able to read only one
    // package per read. So when you sent two packages in one write, it couldn't read both.
    // p.s. packages should be small enough in this test or socket won't be able to transfer them at once
    // and NetExecutor will receive more notifications about new data in socket. In such case it will read packages
    // successfully.

    const int PACKAGES_COUNT = 4;
    QSignalSpy spy(m_executor.data(), &NodeJsExecutor::commandReceived);

    int i = 0;
    while (i < PACKAGES_COUNT) {
        if ((i + 1) < PACKAGES_COUNT) {
            quint32 firstPackageSize = packageSize(i);
            quint32 secondPackageSize = packageSize(++i);
            sendTwoPackagesInOneWrite(firstPackageSize, secondPackageSize);
        } else {
            sendPackageInOneWrite(packageSize(i));
        }
        ++i;
    }

    waitAndCheckPackagesReceived(spy, PACKAGES_COUNT);
}

QTEST_MAIN(TestNetExecutorSocket)
#include "test-netexecutor-socket.moc"
