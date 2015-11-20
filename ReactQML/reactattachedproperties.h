#ifndef REACTATTACHEDPROPERTIES_H
#define REACTATTACHEDPROPERTIES_H

#include <QObject>
#include <QtQml>
#include <QScopedPointer>


class QQuickItem;

class ReactAttachedPropertiesPrivate;
class ReactAttachedProperties : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int tag READ tag WRITE setTag NOTIFY tagChanged)

  Q_DECLARE_PRIVATE(ReactAttachedProperties)

public:
  ReactAttachedProperties(QObject* parent = 0);
  ~ReactAttachedProperties();

  int tag() const;
  void setTag(int tag);

  static ReactAttachedProperties* get(QQuickItem* item);
  static ReactAttachedProperties* qmlAttachedProperties(QObject* object);

Q_SIGNALS:
  void tagChanged();

private:
  QScopedPointer<ReactAttachedPropertiesPrivate> d_ptr;
};

QML_DECLARE_TYPEINFO(ReactAttachedProperties, QML_HAS_ATTACHED_PROPERTIES)

#endif // REACTATTACHEDPROPERTIES_H
