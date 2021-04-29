#ifndef SCROLLVIEWMODEL_H
#define SCROLLVIEWMODEL_H

#include <QAbstractListModel>
#include <QQuickItem>
#include <QSharedPointer>

class ScrollViewModel : public QAbstractListModel {
    Q_OBJECT

public slots:
    int count() const;

public:
    ScrollViewModel(QQmlEngine* engine, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void insert(QQuickItem* item, int position);
    QVariant takeAt(int position);

private:
    QVariantList m_model;
    QQmlEngine* m_engine = nullptr;
};

using ScrollViewModelPtr = QSharedPointer<ScrollViewModel>;

#endif // SCROLLVIEWMODEL_H
