#include "cpustatetableview.h"
#include <iostream>
#include <sstream>

CPUStateTableView::CPUStateTableView(QObject *parent)
    : QAbstractItemModel(parent) {
}

CPUStateTableView::~CPUStateTableView() {
    _rows.clear();
}

int CPUStateTableView::columnCount(const QModelIndex &parent) const {
    return 2;
}

QVariant CPUStateTableView::data(const QModelIndex &index, int role) const {
    if (_rows.size() <= 0)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    if (index.column() == 0)
        return QString(_rows.at(index.row()).first.c_str());
    else if (index.column() == 1) {
        std::stringstream ss;
        ss << std::oct << _rows.at(index.row()).second;
        return QString(ss.str().c_str());
    } else
        return QVariant();
}

Qt::ItemFlags CPUStateTableView::flags(const QModelIndex &index) const {
    if (_rows.size() <= 0)
        return 0;
    return QAbstractItemModel::flags(index);
}

QVariant CPUStateTableView::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Orientation::Horizontal && section == 0 && role == Qt::DisplayRole)
        return "Name";
    if (orientation == Qt::Orientation::Horizontal && section == 1 && role == Qt::DisplayRole)
        return "Value";
    return QVariant();
}

QModelIndex CPUStateTableView::index(int row, int column, const QModelIndex &parent) const {
    return createIndex(row, column);
}

QModelIndex CPUStateTableView::parent(const QModelIndex &index) const {
    return QModelIndex();
}

int CPUStateTableView::rowCount(const QModelIndex &parent) const {
    return (int) _rows.size();
}

void CPUStateTableView::addObject(std::pair<std::string, uint> value) {
    _rows.push_back(value);
}
