#ifndef DISASMTABLEVIEW_H
#define DISASMTABLEVIEW_H

#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <vector>
#include <utility>

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
    int get_selected_row() { return _selected_row; }
    void select_row(int row) { _selected_row = row; }

    void addObject(std::pair<uint, std::string> value);
    void setObjects(std::vector<std::pair<uint, std::string> > rows) { this->_rows = rows; }
private:
    std::vector<std::pair<uint, std::string> > _rows;
    int _selected_row;
};

#endif // DISASMTABLEVIEW_H
