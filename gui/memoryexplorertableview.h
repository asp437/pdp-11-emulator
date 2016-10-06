#ifndef MEMORYEXPLORERTABLEVIEW_H
#define MEMORYEXPLORERTABLEVIEW_H

#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <vector>
#include <utility>

class MemoryExplorerTableView : public QAbstractItemModel {
Q_OBJECT

public:
    explicit MemoryExplorerTableView(QObject *parent = 0);
    ~MemoryExplorerTableView();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    void setObjects(std::vector<uint16_t> data) { this->_data = data; }
private:
    std::vector<uint16_t> _data;
};

#endif // MEMORYEXPLORERTABLEVIEW_H
