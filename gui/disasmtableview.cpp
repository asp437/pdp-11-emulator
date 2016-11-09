#include "disasmtableview.h"
#include <iostream>
#include <QBrush>
#include <QColor>
#include <sstream>
#include <iomanip>
#include <QtWidgets/QTableWidgetItem>

DisasmTableView::DisasmTableView(QObject *parent)
    : QAbstractItemModel(parent) {
}

DisasmTableView::~DisasmTableView() {
    _rows.clear();
    _checks.clear();
}

int DisasmTableView::columnCount(const QModelIndex &parent) const {
    return 3;
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

    if (role != Qt::DisplayRole) {
        if (role == Qt::CheckStateRole) {
            if (index.column() == 0 && index.row() < _checks.size()) {
                return _checks.at(index.row()) ? Qt::Checked : Qt::Unchecked;
            }
        }
        return QVariant();
    }
    if (index.column() == 0 && _rows.size() > index.row()) {
        return QVariant();
    } else if (index.column() == 1 && _rows.size() > index.row()) {
        std::stringstream stream;
        stream << std::oct << std::setfill('0') << std::setw(6) << _rows.at(index.row()).first;
        return QString(stream.str().c_str());
    } else if (index.column() == 2 && _rows.size() > index.row())
        return QString(_rows.at(index.row()).second.first.c_str());
    else
        return QVariant();
}

bool DisasmTableView::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.column() == 0 && _rows.size() > index.row()) {
        _checks[index.row()] = value.toBool();
    }
    return true;
}

Qt::ItemFlags DisasmTableView::flags(const QModelIndex &index) const {
    if (_rows.size() <= 0)
        return 0;
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.column() == 0)
        flags.setFlag(Qt::ItemFlag::ItemIsUserCheckable, true); //.setFlag(Qt::ItemFlag::ItemIsEditable, true);
    return flags;
}

QVariant DisasmTableView::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Orientation::Horizontal && section == 0 && role == Qt::DisplayRole)
        return "BP";
    if (orientation == Qt::Orientation::Horizontal && section == 1 && role == Qt::DisplayRole)
        return "Address";
    if (orientation == Qt::Orientation::Horizontal && section == 2 && role == Qt::DisplayRole)
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
    return (int) _rows.size();
}

void DisasmTableView::setObjects(uint16 base_address, std::vector<std::pair<std::string, uint16>> &rows) {
    _rows.clear();
    _checks.clear();
    uint current_address = base_address;
    for (auto it = rows.begin(); it != rows.end(); ++it) {
        pair<string, uint> instruction = *it;
        _rows.push_back(make_pair(current_address, instruction));
        current_address += instruction.second;
    }
    _checks.resize(_rows.size());
}

bool DisasmTableView::isBreakpointAddreess(uint16 address) {
    for (int i = 0; i < _rows.size(); i++) {
        if (_rows[i].first == address) {
            return _checks[i] == 1;
        }
    }
    return false;
}

vector<uint16> DisasmTableView::getBreakpointsList() {
    vector<uint16> result;
    for (int i = 0; i < _rows.size(); i++) {
        if (_checks[i]) {
            result.push_back(_rows[i].first);
        }
    }
    return result;
}