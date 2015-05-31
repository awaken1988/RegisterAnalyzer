#ifndef REGISTERFIELDMODEL_H
#define REGISTERFIELDMODEL_H

#include <QDebug>
#include <QAbstractTableModel>
#include <QString>
#include <QBitArray>
#include <QColor>
#include <tuple>
#include <vector>
#include <algorithm>
#include <tuple>
#include "bitconverter.h"
#include "coloring.h"

struct field_t
{
    QString     name;
    int         start;
    int         end;
    QBitArray   extracted;
    QColor      color;

    static const int INVALID_POS = -1;

    field_t();
    field_t(QString aName, int aStart, int aEnd, QColor aColor=QColor((QRgb)0x00000000));

    bool isValidRange();
};

class RegisterFieldModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum class field_map_e : int
    {
        Name            = 0,
        StartPos        = 1,
        EndPos          = 2,
        ExtractedValue  = 3,
        LAST,
    };

    using fieldlist_t   = std::vector<field_t>;



    explicit RegisterFieldModel(QObject *parent = 0);
    virtual ~RegisterFieldModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const ;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    const QBitArray& getRegister() const;
    int         getBitArraySize();
    const fieldlist_t& getField();
    bool addRow(QString aName);
    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex()) override;

    bool import(RegisterFieldModel& aRegModel);

protected:
    fieldlist_t     m_bitFields;
    QBitArray       m_content;
    QString         m_registerName;

    NsColoring::ColorGenerator m_colorGen;
signals:
    void rangeChanded();
    void fieldChanged(const QBitArray&);                       //new model
    void nameChanged(QString);                                 //register name changed
public slots:
    void updateRegisterValue(const QBitArray& aContent);       //new model
    void setRegisterName(QString aName);
};

#endif // REGISTERFIELDMODEL_H
