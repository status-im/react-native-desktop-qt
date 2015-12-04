
#include <QMetaObject>
#include <QMetaProperty>
#include <QQuickItem>

#include <QDebug>

#include "reactevents.h"
#include "reactcomponentdata.h"
#include "reactviewmanager.h"


ReactComponentData::ReactComponentData(ReactViewManager* viewManager)
  : m_viewManager(viewManager)
{
}

ReactComponentData::~ReactComponentData()
{
}

QString ReactComponentData::name() const
{
  QString mn = m_viewManager->moduleName();
  int mi = mn.indexOf("Manager");
  if (mi != -1)
    return mn.left(mi);

  return mn;
}

ReactViewManager* ReactComponentData::manager() const
{
  return m_viewManager;
}

QVariantMap ReactComponentData::viewConfig() const
{
  qDebug() << __PRETTY_FUNCTION__ << name();

  QVariantMap rc;

  // Create a temporary view to inspect, oh well
  QQuickItem* view = m_viewManager->view();
  if (view == nullptr) {
    qDebug() << name() << "has no view for inspecting!";
    return rc;
  }
  view->deleteLater();

  const QMetaObject* metaObject = view->metaObject();
  const int propertyCount = metaObject->propertyCount();

  QVariantMap propTypes;
  for (int i = metaObject->propertyOffset(); i < propertyCount; ++i) {
    QMetaProperty p = metaObject->property(i);
    propTypes.insert(p.name(), p.typeName());
  }

  rc.insert("propTypes", propTypes);

  // Events
  QStringList de = m_viewManager->customDirectEventTypes();
  QStringList dep;
  std::transform(de.begin(), de.end(),
                 std::back_inserter(dep),
                 [](const QString& name) { return normalizeInputEventName(name); });
  rc.insert("directEvents", dep);

  de = m_viewManager->customBubblingEventTypes();
  dep.clear();
  std::transform(de.begin(), de.end(),
                 std::back_inserter(dep),
                 [](const QString& name) { return normalizeInputEventName(name); });
  rc.insert("bubblingEvents", dep);

  return rc;
}

QQuickItem* ReactComponentData::createView(int tag, const QVariantMap& properties)
{
  // qDebug() << __PRETTY_FUNCTION__ << tag << properties;
  return m_viewManager->view(properties);
}

