#ifndef REACTVIEW_H
#define REACTVIEW_H

#include <QString>
#include <QUrl>

#include "reactitem.h"


class ReactBridge;

class ReactView : public ReactItem
{
  Q_OBJECT
  Q_PROPERTY(QString moduleName READ moduleName WRITE setModuleName NOTIFY moduleNameChanged)
  Q_PROPERTY(QUrl codeLocation READ codeLocation WRITE setCodeLocation NOTIFY codeLocationChanged)
  Q_PROPERTY(QVariantMap properties READ properties WRITE setProperties NOTIFY propertiesChanged)

public:
  ReactView(QQuickItem *parent = 0);
  ~ReactView();

  QString moduleName() const;
  void setModuleName(const QString& moduleName);

  QUrl codeLocation() const;
  void setCodeLocation(const QUrl& codeLocation);

  QVariantMap properties() const;
  void setProperties(const QVariantMap& properties);

Q_SIGNALS:
  void moduleNameChanged();
  void codeLocationChanged();
  void propertiesChanged();

private Q_SLOTS:
  void bridgeReady();

private:
  void componentComplete() override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  QString m_moduleName;
  QUrl m_codeLocation;
  QVariantMap m_properties;
  ReactBridge* m_bridge;
};

#endif // REACTVIEW_H
