#include "memoryexplorertableview.h"
#include <sstream>
#include <iomanip>

MemoryExplorerTableView::MemoryExplorerTableView(QObject *parent)
    : QAbstractItemModel(parent) {
}

MemoryExplorerTableView::~MemoryExplorerTableView() {
    _data.clear();
}

int MemoryExplorerTableView::columnCount(const QModelIndex &parent) const {
    return 4;
}

QVariant MemoryExplorerTableView::data(const QModelIndex &index, int role) const {
    if (_data.size() <= 0)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    if (index.column() >= 0) {
        if (index.row() * 04 + index.column() < _data.size()) {
            std::stringstream stream;
            stream << std::oct << std::setfill('0') << std::setw(6) << _data.at(index.row() * 04 + index.column());
            return QString(stream.str().c_str());
        } else {
            return QString("000000");
        }
    } else {
        return QVariant();
    }
}

Qt::ItemFlags MemoryExplorerTableView::flags(const QModelIndex &index) const {
    if (_data.size() <= 0)
        return 0;
    return QAbstractItemModel::flags(index);
}

QVariant MemoryExplorerTableView::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole && section >= 0 && section <= 3) {
        std::stringstream stream;
        stream << "xxxxx" << std::oct << section * 02;
        return QString(stream.str().c_str());
    }
    if (orientation == Qt::Orientation::Vertical && role == Qt::DisplayRole && section >= 0) {
        std::stringstream stream;
        stream << std::oct << std::setfill('0') << std::setw(5) << section + _base_address / 8 << "x";
        return QString(stream.str().c_str());
    }
    return QVariant();
}

QModelIndex MemoryExplorerTableView::index(int row, int column, const QModelIndex &parent) const {
    return createIndex(row, column);
}

QModelIndex MemoryExplorerTableView::parent(const QModelIndex &index) const {
    return QModelIndex();
}

int MemoryExplorerTableView::rowCount(const QModelIndex &parent) const {
    return (int) (_data.size() / 4.0 + 1);
}
