
#include <QMetaObject>
#include <QMetaProperty>
#include <QQuickItem>

#include <QDebug>

#include "reactevents.h"
#include "reactcomponentdata.h"
#include "reactviewmanager.h"
#include "reactattachedproperties.h"
#include "reactpropertyhandler.h"


ReactComponentData::ReactComponentData(ReactModuleInterface* moduleInteface)
  : m_moduleInterface(moduleInteface)
{
}

ReactComponentData::~ReactComponentData()
{
}

QString ReactComponentData::name() const
{
  QString mn = m_moduleInterface->viewManager()->moduleName();
  int mi = mn.indexOf("Manager");
  if (mi != -1)
    return mn.left(mi);

  return mn;
}

ReactViewManager* ReactComponentData::manager() const
{
  return m_moduleInterface->viewManager();
}

QList<QMetaProperty> findProperties(QObject* object)
{
  QList<QMetaProperty> properties;

  const QMetaObject* metaObject = object->metaObject();
  const int propertyCount = metaObject->propertyCount();

  for (int i = metaObject->propertyOffset(); i < propertyCount; ++i) {
    QMetaProperty p = metaObject->property(i);
    if (p.isScriptable())
      properties.push_back(p);
  }

  return properties;
}

QVariantMap ReactComponentData::viewConfig() const
{
  qDebug() << __PRETTY_FUNCTION__ << name();

  QVariantMap rc;

  // Create a temporary view to inspect, oh well
  QQuickItem* view = m_moduleInterface->viewManager()->view();
  if (view == nullptr) {
    qDebug() << name() << "has no view for inspecting!";
    return rc;
  }
  view->deleteLater();

  ReactPropertyHandler* ph = m_moduleInterface->propertyHandler(view);
  ph->deleteLater();

  // {{{ propTypes
  QList<QMetaProperty> properties = ph->availableProperties();

  QVariantMap propTypes;
  for (auto p : properties) {
    propTypes.insert(p.name(), p.typeName());
  }

  if (name() == "UbuntuPage") {
    // XXX: need a refactor because
    propTypes.insert("title", "QString");
  }

  rc.insert("propTypes", propTypes);
  // }}}

  // Events
  QStringList de = m_moduleInterface->viewManager()->customDirectEventTypes();
  QStringList dep;
  std::transform(de.begin(), de.end(),
                 std::back_inserter(dep),
                 [](const QString& name) { return normalizeInputEventName(name); });
  rc.insert("directEvents", dep);

  de = m_moduleInterface->viewManager()->customBubblingEventTypes();
  dep.clear();
  std::transform(de.begin(), de.end(),
                 std::back_inserter(dep),
                 [](const QString& name) { return normalizeInputEventName(name); });
  rc.insert("bubblingEvents", dep);

  return rc;
}

QQuickItem* ReactComponentData::createView(int tag, const QVariantMap& properties)
{
  ReactViewManager* viewManager = m_moduleInterface->viewManager();

  QQuickItem* view = viewManager->view(properties);

  ReactAttachedProperties* rap = ReactAttachedProperties::get(view);
  rap->setTag(tag);
  rap->setShouldLayout(viewManager->shouldLayout());
  rap->setViewManager(viewManager);
  rap->setPropertyHandler(m_moduleInterface->propertyHandler(view));

  return view;
}
