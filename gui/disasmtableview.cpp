#include "disasmtableview.h"
#include <iostream>
#include <QBrush>
#include <QColor>
#include <sstream>
#include <iomanip>

DisasmTableView::DisasmTableView(QObject *parent)
    : QAbstractItemModel(parent) {
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
        if (_rows[index.row()].first == _current_address)
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
        return QString(_rows.at(index.row()).second.first.c_str());
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

void DisasmTableView::setObjects(uint base_address, std::vector<std::pair<std::string, uint16>> &rows) {
    uint current_address = base_address;
    for (auto it = rows.begin(); it != rows.end(); ++it) {
        pair<string, uint> instruction = *it;
        _rows.push_back(make_pair(current_address, instruction));
        current_address += instruction.second;
    }
}
