#include "disasmtableview.h"
#include <iostream>
#include <QBrush>
#include <QColor>
#include <sstream>
#include <iomanip>

DisasmTableView::DisasmTableView(QObject *parent)
    : QAbstractItemModel(parent) {
    _selected_row = -1;
}

DisasmTableView::~DisasmTableView() {
    _rows.clear();
}

int DisasmTableView::columnCount(const QModelIndex &parent) const {
    return 2;
}

QVariant DisasmTableView::data(const QModelIndex &index, int role) const {
    if (_rows.size() <= 0)
        return QVariant();
    if (role == Qt::BackgroundRole) {
        if (index.row() == _selected_row)
            return QBrush(QColor(82, 179, 217));
        else
            return QBrush(QColor(255, 255, 255));
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    if (index.column() == 0) {
        std::stringstream stream;
        stream << std::oct << std::setfill('0') << std::setw(7) << _rows.at(index.row()).first;
        return QString(stream.str().c_str());
    } else if (index.column() == 1)
        return QString(_rows.at(index.row()).second.c_str());
    else
        return QVariant();
}

Qt::ItemFlags DisasmTableView::flags(const QModelIndex &index) const {
    if (_rows.size() <= 0)
        return 0;
    return QAbstractItemModel::flags(index).setFlag(Qt::ItemIsSelectable, false);
}

QVariant DisasmTableView::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Orientation::Horizontal && section == 0 && role == Qt::DisplayRole)
        return "Address";
    if (orientation == Qt::Orientation::Horizontal && section == 1 && role == Qt::DisplayRole)
        return "Mnemonic";
    return QVariant();
}

QModelIndex DisasmTableView::index(int row, int column, const QModelIndex &parent) const {
    return createIndex(row, column);
}

QModelIndex DisasmTableView::parent(const QModelIndex &index) const {
    return QModelIndex();
}

int DisasmTableView::rowCount(const QModelIndex &parent) const {
    return _rows.size();
}

void DisasmTableView::addObject(std::pair<uint, std::string>  value) {
    _rows.push_back(value);
}
