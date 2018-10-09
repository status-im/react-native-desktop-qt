#include "scrollviewmodel.h"
#include <QDebug>

ScrollViewModel::ScrollViewModel(QQmlEngine* engine, QObject* parent) : QAbstractListModel(parent) {
    m_engine = engine;
}

int ScrollViewModel::rowCount(const QModelIndex& parent) const {
    return count();
}

int ScrollViewModel::count() const {
    return m_model.size();
}

QVariant ScrollViewModel::data(const QModelIndex& index, int role) const {
    Q_UNUSED(role)
    if (!hasIndex(index.row(), index.column(), index.parent()))
        return {};

    return m_model[index.row()];
}

void ScrollViewModel::insert(QQuickItem* item, int position) {
    beginInsertRows(QModelIndex(), position, position);
    m_model.insert(position, QVariant::fromValue(item));
    endInsertRows();
}

QVariant ScrollViewModel::takeAt(int position) {
    beginRemoveRows(QModelIndex(), position, position);
    QVariant result = m_model.takeAt(position);
    endRemoveRows();
    return result;
}
