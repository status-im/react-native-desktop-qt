
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

#include <QString>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <QDebug>

#include "ubuntudatepickermanager.h"
#include "reactbridge.h"
#include "reactuimanager.h"
#include "reactattachedproperties.h"
#include "reactpropertyhandler.h"
#include "reactevents.h"
#include "reactflexlayout.h"


class DatePickerPropertyHandler : public ReactPropertyHandler {
  Q_OBJECT
  Q_PROPERTY(QDateTime date READ date WRITE setDate)
  Q_PROPERTY(bool onChange READ onChange WRITE setOnChange)
  Q_PROPERTY(QDateTime maximumDate READ maximumDate WRITE setMaximumDate)
  Q_PROPERTY(QDateTime miniumDate READ minimumDate WRITE setMinimumDate)
  Q_PROPERTY(QString mode READ mode WRITE setMode)
  Q_PROPERTY(int minuteInterval READ minuteInterval WRITE setMinuteInterval)
  Q_PROPERTY(int timeZoneOffsetInMinutes READ timeZoneOffsetInMinutes WRITE setTimeZoneOffsetInMinutes)
  Q_PROPERTY(bool qmlImplicitWidth READ qmlImplicitWidth WRITE setQmlImplicitWidth)
  Q_PROPERTY(bool qmlImplicitHeight READ qmlImplicitHeight WRITE setQmlImplicitHeight)

public:
  DatePickerPropertyHandler(QObject* object)
    : ReactPropertyHandler(object)
    , m_onChange(false) {
    }
  QDateTime date() const;
  void setDate(const QDateTime& date);
  bool onChange() const;
  void setOnChange(bool onChanage);
  bool m_onChange;
  QDateTime maximumDate() const;
  void setMaximumDate(const QDateTime& maximumDate);
  QDateTime minimumDate() const;
  void setMinimumDate(const QDateTime& minimumDate);
  QString mode() const;
  void setMode(const QString& mode);
  int minuteInterval() const;
  void setMinuteInterval(int minimumInterval);
  int timeZoneOffsetInMinutes() const;
  void setTimeZoneOffsetInMinutes(int timeZoneOffsetInMinutes);
  bool qmlImplicitWidth() const;
  void setQmlImplicitWidth(bool qmlImplicitWidth);
  bool qmlImplicitHeight() const;
  void setQmlImplicitHeight(bool qmlImplicitHeight); };

QDateTime DatePickerPropertyHandler::date() const
{
  return QQmlProperty(m_object, "date").read().toDateTime();
}

void DatePickerPropertyHandler::setDate(const QDateTime& date)
{
  QQmlProperty(m_object, "date").write(date);
}

bool DatePickerPropertyHandler::onChange() const
{
  return m_onChange;
}

void DatePickerPropertyHandler::setOnChange(bool onChange)
{
  m_onChange = onChange;
}

QDateTime DatePickerPropertyHandler::maximumDate() const
{
  return QQmlProperty(m_object, "maximum").read().value<QDateTime>();
}

void DatePickerPropertyHandler::setMaximumDate(const QDateTime& maximumDate)
{
  QQmlProperty(m_object, "maximum").write(maximumDate);
}

QDateTime DatePickerPropertyHandler::minimumDate() const
{
  return QQmlProperty(m_object, "minimum").read().value<QDateTime>();
}

void DatePickerPropertyHandler::setMinimumDate(const QDateTime& minimumDate)
{
  QQmlProperty(m_object, "minimum").write(minimumDate);
}

QString DatePickerPropertyHandler::mode() const
{
  return QString();
}

void DatePickerPropertyHandler::setMode(const QString& mode)
{
  if (mode == "date") {
    QQmlProperty(m_object, "mode").write("Years|Months|Days");
  } else if (mode == "time") {
    QQmlProperty(m_object, "mode").write("Hours|Minutes|Seconds");
  } else if (mode == "datetime") {
    // XXX: not supported!
  }
}

int DatePickerPropertyHandler::minuteInterval() const
{
  return int();
}

void DatePickerPropertyHandler::setMinuteInterval(int minimumInterval)
{
  // XXX: not supported
}

int DatePickerPropertyHandler::timeZoneOffsetInMinutes() const
{
  return int();
}

void DatePickerPropertyHandler::setTimeZoneOffsetInMinutes(int timeZoneOffsetInMinutes)
{
  // TODO: locale
}

bool DatePickerPropertyHandler::qmlImplicitWidth() const
{
  ReactFlexLayout::get(qobject_cast<QQuickItem*>(m_object))->qmlImplicitWidth();
}

void DatePickerPropertyHandler::setQmlImplicitWidth(bool qmlImplicitWidth)
{
  ReactFlexLayout::get(qobject_cast<QQuickItem*>(m_object))->setQmlImplicitWidth(qmlImplicitWidth);
}

bool DatePickerPropertyHandler::qmlImplicitHeight() const
{
  ReactFlexLayout::get(qobject_cast<QQuickItem*>(m_object))->qmlImplicitHeight();
}

void DatePickerPropertyHandler::setQmlImplicitHeight(bool qmlImplicitHeight)
{
  ReactFlexLayout::get(qobject_cast<QQuickItem*>(m_object))->setQmlImplicitHeight(qmlImplicitHeight);
}


UbuntuDatePickerManager::UbuntuDatePickerManager(QObject* parent)
  : ReactViewManager(parent)
  , m_id(0)
{
}

UbuntuDatePickerManager::~UbuntuDatePickerManager()
{
}

void UbuntuDatePickerManager::setBridge(ReactBridge* bridge)
{
  m_bridge = bridge;
}

ReactViewManager* UbuntuDatePickerManager::viewManager()
{
  return this;
}

ReactPropertyHandler* UbuntuDatePickerManager::propertyHandler(QObject* object)
{
  Q_ASSERT(qobject_cast<QQuickItem*>(object) != nullptr);
  return new DatePickerPropertyHandler(object);
}

QString UbuntuDatePickerManager::moduleName()
{
  return "UbuntuDatePickerManager";
}

QList<ReactModuleMethod*> UbuntuDatePickerManager::methodsToExport()
{
  return QList<ReactModuleMethod*>{};
}

QVariantMap UbuntuDatePickerManager::constantsToExport()
{
  return QVariantMap{};
}

QStringList UbuntuDatePickerManager::customBubblingEventTypes()
{
  return QStringList{ normalizeInputEventName("onChange") };
}

namespace {
static const char* component_qml = R"COMPONENT(
import QtQuick 2.4
import Ubuntu.Components.Pickers 1.0

DatePicker {
  id: datepicker%1
}
)COMPONENT";
}

QQuickItem* UbuntuDatePickerManager::view(const QVariantMap& properties) const
{
  QString componentString = QString(component_qml).arg(m_id++);

  QQmlComponent component(m_bridge->qmlEngine());
  component.setData(componentString.toLocal8Bit(), QUrl());
  if (!component.isReady())
    qCritical() << "Component for UbuntuDatePickerManager not ready" <<
              componentString << component.errors();

  QQuickItem* item = qobject_cast<QQuickItem*>(component.create());
  if (item == nullptr) {
    qCritical() << "Unable to create component for UbuntuDatePickerManager";
    return nullptr;
  }

  configureView(item);

  return item;
}

void UbuntuDatePickerManager::onDateChanged()
{
  ReactAttachedProperties* ap = ReactAttachedProperties::get(qobject_cast<QQuickItem*>(sender()));
  if (ap == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "failed to find ReactAttachedProperties";
    return;
  }
  DatePickerPropertyHandler* ph = qobject_cast<DatePickerPropertyHandler*>(ap->propertyHandler());
  if (ph == nullptr) {
    qCritical() << __PRETTY_FUNCTION__ << "failed to find DatePickerPropertyHandler";
    return;
  }
  if (ph->onChange()) {
    m_bridge->enqueueJSCall("RCTEventEmitter", "receiveEvent",
                            QVariantList{ap->tag(),
                                         normalizeInputEventName("onChange"),
                                         QVariantMap{{"timestamp", ph->date().toMSecsSinceEpoch()}}});
  }
}

void UbuntuDatePickerManager::configureView(QQuickItem* view) const
{
  connect(view, SIGNAL(dateChanged()), SLOT(onDateChanged()));
}

#include "ubuntudatepickermanager.moc"
