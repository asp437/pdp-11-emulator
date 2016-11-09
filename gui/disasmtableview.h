#ifndef DISASMTABLEVIEW_H
#define DISASMTABLEVIEW_H

#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <vector>
#include <utility>
#include "../common.h"

class DisasmTableView : public QAbstractItemModel {
Q_OBJECT

public:
    explicit DisasmTableView(QObject *parent = 0);
    ~DisasmTableView();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool isBreakpointAddreess(uint16 address);
    vector<uint16> getBreakpointsList();

    void setCurrentAddress(uint16 current_address) { _current_address = current_address; }
    void setObjects(uint16 base_address, std::vector<std::pair<std::string, uint16>> &rows);
private:
    std::vector<std::pair<uint16, std::pair<std::string, uint16>>> _rows;
    int _selected_row;
    uint16 _current_address;
    vector<bool> _checks;
};

#endif // DISASMTABLEVIEW_H
