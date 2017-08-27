
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

#include <QAbstractListModel>

#include "reactredboxitem.h"
#include "reactbridge.h"


namespace {
static const char* REDBOX_COMPONENT_QML = R"COMPONENT(
import QtQuick 2.4
import QtQuick.Controls 1.4

Rectangle {
 id: redboxRect
 color: 'red'
 anchors.fill: parent

 property alias message: textMessage.text
 property alias stackModel: stackListView.model

 signal dismissPressed();
 signal reloadPressed();

 Text {
   id: textMessage
   visible: message.length > 0
   anchors {
     centerIn: redboxRect
     margins: 40
   }
   color: 'white'
   font.pointSize: 18
   wrapMode: Text.WordWrap
   onTextChanged: console.log("redbox message:", text)
 }

 ListView {
   id: stackListView
   anchors {
     top: textMessage.bottom
     left: parent.left
     bottom: buttonRow.top
     right: parent.right
     margins: 40
   }

   clip: true

   delegate: Label {
       height: 60
       color: 'white'
       text: methodName + "\n" + file + " @ " + lineNumber + ":" + column
       font.weight: Font.DemiBold
       verticalAlignment: Text.AlignVCenter
     }
   }

 Row {
   id: buttonRow
   anchors {
     bottomMargin: 60
     horizontalCenter: parent.horizontalCenter
     bottom: parent.bottom
   }

   spacing: 40

   Button {
     text: 'Reload JS'
     onClicked: reloadPressed()
   }

   Button {
     text: 'Dismiss'
     onClicked: dismissPressed()
   }
 }

 state: stackModel !== undefined && !stackModel.empty ? "stackTrace" : "errorMessage"
 onStateChanged: {
   // XXX: weird hack
   if (state === 'stackTrace') {
     textMessage.anchors.centerIn = undefined;
   }
 }

 states: [
   State {
     name: "errorMessage"
     PropertyChanges {
       target: textMessage
       anchors.left: undefined
       anchors.top: undefined
       anchors.centerIn: redboxRect
     }
     PropertyChanges {
       target: stackListView
       visible: false
     }
   },
   State  {
     name: "stackTrace"
     PropertyChanges {
       target: textMessage
       anchors.centerIn: undefined
       anchors.left: redboxRect.left
       anchors.top: redboxRect.top
     }
     PropertyChanges {
       target: stackListView
       visible: true
     }
   }
 ]
}
)COMPONENT";

enum Roles {
  LineNumber = Qt::UserRole + 1,
  Column,
  File,
  MethodName,
};

static QHash<int, QByteArray> roleData{
    { LineNumber, "lineNumber" },
    { Column, "column" },
    { File, "file" },
    { MethodName, "methodName" },
  };
}

class ReactRedboxItemPrivate : public QAbstractListModel {

  Q_OBJECT
  Q_PROPERTY(bool empty READ empty NOTIFY emptyChanged)
  Q_DECLARE_PUBLIC(ReactRedboxItem)

public:
  QList<QVariantMap> stack;
  QQuickItem* redbox;
  ReactRedboxItem* q_ptr;
  ReactBridge* bridge;

  ReactRedboxItemPrivate(ReactRedboxItem* q)
    : q_ptr(q)
  {}

  void setStack(const QList<QVariantMap>& stack) {
    const bool oe = this->stack.isEmpty();
    beginResetModel();
    this->stack = stack;
    if (!stack.isEmpty()) {
      beginInsertRows(QModelIndex(), 0, stack.length());
      endInsertRows();
    }
    if (this->stack.isEmpty() != oe)
      Q_EMIT emptyChanged();
    endResetModel();
  }

  int rowCount(const QModelIndex& parent) const override {
    return stack.length();
  }

  QVariant data(const QModelIndex& index, int role) const override {
    return stack.at(index.row()).value(roleData[role]).toString();
  }

  QHash<int, QByteArray> roleNames() const override {
    return roleData;
  }

  bool empty() const {
    return stack.isEmpty();
  }


  void createRedboxItem(QQuickItem* parent) {

    QQmlComponent component(bridge->qmlEngine());
    component.setData(REDBOX_COMPONENT_QML, QUrl::fromLocalFile(__FILE__));
    redbox = qobject_cast<QQuickItem*>(component.create());
    if (redbox == nullptr) {
      qCritical() << __PRETTY_FUNCTION__ << "Unable to create RedboxItem" << component.errors();
      return;
    }

    redbox->setObjectName("Redbox");
    redbox->setParentItem(parent);
    redbox->setProperty("stackModel", QVariant::fromValue(this));

    connect(redbox, SIGNAL(reloadPressed()), this, SLOT(reloadPressed()));
    connect(redbox, SIGNAL(dismissPressed()), this, SLOT(dismissPressed()));
  }


public Q_SLOTS:

  void reloadPressed() {
    q_ptr->setParentItem(0);
    bridge->reload();
  }

  void dismissPressed() {
    q_ptr->setParentItem(0);
  }

Q_SIGNALS:
  void emptyChanged();
};


ReactRedboxItem::ReactRedboxItem(ReactBridge* bridge)
  : QQuickItem(0)
  , d_ptr(new ReactRedboxItemPrivate(this))
{
  Q_D(ReactRedboxItem);
  d->bridge = bridge;
  d->createRedboxItem(this);
}

ReactRedboxItem::~ReactRedboxItem()
{
}

void ReactRedboxItem::showErrorMessage(const QString& message, const QList<QVariantMap>& stack)
{
  Q_D(ReactRedboxItem);
  d->redbox->setProperty("message", message);
  d->setStack(stack);

  QQuickItem* rootView = d->bridge->visualParent();
  setParentItem(rootView);

  setX(0);
  setY(0);
  setWidth(rootView->width());
  setHeight(rootView->height());
}

void ReactRedboxItem::updateErrorMessage(const QString& message, const QList<QVariantMap>& stack)
{
  Q_D(ReactRedboxItem);
  d->redbox->setProperty("message", message);
  d->setStack(stack);
}

#include "reactredboxitem.moc"
