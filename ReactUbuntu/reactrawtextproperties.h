#ifndef REACTRAWTEXTPROPERTIES_H
#define REACTRAWTEXTPROPERTIES_H

#include <QString>
#include <QColor>
#include <QObject>
#include <QtQml>
#include <QScopedPointer>

#include "reactpropertyhandler.h"


class QQuickItem;

class ReactRawTextPropertiesPrivate;
class ReactRawTextProperties : public ReactPropertyHandler
{
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText)
  Q_DECLARE_PRIVATE(ReactRawTextProperties);

public:
  ReactRawTextProperties(QObject* parent = 0);
  ~ReactRawTextProperties();

  QString text() const;
  void setText(const QString& text);

  QString textWithProperties(const QVariantMap& properties) const;

  static ReactRawTextProperties* get(QQuickItem* item, bool create = true);
  static ReactRawTextProperties* qmlAttachedProperties(QObject* object);

private:
  QScopedPointer<ReactRawTextPropertiesPrivate> d_ptr;
};

QML_DECLARE_TYPEINFO(ReactRawTextProperties, QML_HAS_ATTACHED_PROPERTIES)

#endif // REACTTEXTPROPERTIES_H
