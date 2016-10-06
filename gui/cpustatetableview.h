#ifndef CPUSTATETABLEVIE_H
#define CPUSTATETABLEVIE_H

#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <vector>
#include <utility>

class CPUStateTableView : public QAbstractItemModel {
Q_OBJECT

public:
    explicit CPUStateTableView(QObject *parent = 0);
    ~CPUStateTableView();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    void addObject(std::pair<std::string, uint>  value);
    void setObjects(std::vector<std::pair<std::string, uint> >  rows) { this->_rows = rows; }
private:
    std::vector<std::pair<std::string, uint> > _rows;
};

#endif // CPUSTATETABLEVIE_H
