#ifndef REACTPROPERTYHANDLER_H
#define REACTPROPERTYHANDLER_H

#include <QMap>
#include <QObject>
#include <QMetaProperty>


class QQuickItem;


class ReactPropertyHandler : public QObject
{
  Q_OBJECT

public:
  ReactPropertyHandler(QObject* object, bool exposeQmlProperties = true);
  virtual ~ReactPropertyHandler();

  virtual QList<QMetaProperty> availableProperties();
  virtual void applyProperties(const QVariantMap& properties);

protected:
  bool m_exposeQmlProperties;
  bool m_cached = false;
  QObject* m_object;
  QMap<QString, QMetaProperty> m_coreProperties;
  QMap<QString, QMetaProperty> m_extraProperties;

private:
  void buildPropertyMap();
};

#endif // REACTPROPERTYHANDLER_H
