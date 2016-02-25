#include "registerfieldmodel.h"

/*
 *      field_t
 */
field_t::field_t()
    : name(""), start(0), end(0), color((QRgb)0xFFFFFFFF)
{

}

field_t::field_t(QString aName, int aStart, int aEnd, QColor aColor)
    : name(aName), start(aStart), end(aEnd), color(aColor)
{

}

bool field_t::isValidRange()
{
    //redundant but more expressive
    if( INVALID_POS == start || INVALID_POS == end )
        return false;

    if( start < 0 || end < 0 )
        return false;

    if( start > end )
        return false;

    return true;
}

/*
 *      RegisterFieldModel
 */


RegisterFieldModel::RegisterFieldModel(QObject *parent) :
    QAbstractTableModel(parent)
{

    m_bitFields.push_back(field_t("byte0", 0, 7,    m_colorGen.getNextColor()));
    m_bitFields.push_back(field_t("byte1", 8, 15,    m_colorGen.getNextColor()));
    m_bitFields.push_back(field_t("byte2", 16, 23,   m_colorGen.getNextColor()));
    m_bitFields.push_back(field_t("byte3", 24, 31,  m_colorGen.getNextColor()));
}

RegisterFieldModel::~RegisterFieldModel()
{
    qDebug("RegisterFieldModel obj destroyed");
}

 int RegisterFieldModel::rowCount(const QModelIndex & /*parent*/) const
 {
    return m_bitFields.size();
 }

 int RegisterFieldModel::columnCount(const QModelIndex & /*parent*/) const
 {
     return (int)field_map_e::LAST;
 }

 QVariant RegisterFieldModel::data(const QModelIndex &index, int role) const
 {
     if (role == Qt::DisplayRole)
     {
        if( (unsigned)index.row() >= m_bitFields.size() )
            return QString("ILLEGAL ROW");

        const field_t& rowField = m_bitFields.at(index.row());

        switch(index.column())
        {
            case ((int)field_map_e::Name):              return rowField.name;
            case ((int)field_map_e::StartPos):          return rowField.start;
            case ((int)field_map_e::EndPos):            return rowField.end;
            case ((int)field_map_e::ExtractedValue):    return NsConverter::toHexStr(rowField.extracted);
        }
     }

     if( role == Qt::BackgroundRole
             && index.column() != (int)field_map_e::Name
             && index.column() != (int)field_map_e::ExtractedValue )
     {
         return QVariant(m_bitFields[index.row()].color);
     }

     return QVariant();
 }

 bool RegisterFieldModel::setData(const QModelIndex & aIndex, const QVariant & value, int role)
 {
      qDebug()<<__PRETTY_FUNCTION__;

     int row = aIndex.row();
     int column = aIndex.column();

     if (role == Qt::EditRole)
     {
         switch(aIndex.column())
         {
            case (int)field_map_e::Name:
                                                    for(const auto iRow: m_bitFields )
                                                    {
                                                        if( iRow.name == value.toString()  )
                                                            return false;
                                                    }
                                                    m_bitFields[row].name   = value.toString();
                                                    emit dataChanged(aIndex, aIndex);
                                                    break;

            case (int)field_map_e::StartPos:        /*no break*/
            case (int)field_map_e::EndPos:
                {
                    bool isNum;
                    int pos = value.toUInt(&isNum);

                    if(!isNum)
                        return false;

                    if( (int)field_map_e::StartPos == column  )
                        m_bitFields[row].start = pos;
                    else
                        m_bitFields[row].end = pos;

                    updateRegisterValue(m_content);
                    break;
                }
            case (int)field_map_e::ExtractedValue:   /*emit fieldChanged();*/
                {
                    QString strVal = value.toString();
                    QBitArray bitVal;
                    const int startPos = m_bitFields[row].start;
                    const int endPos   = m_bitFields[row].end;

                    if( !NsConverter::fromHexStr(bitVal, strVal, true) )
                        return false;
                    if( -1 == startPos || -1 == endPos)
                        return false;

                    //crop bits
                    bitVal.resize(endPos-startPos+1);

                    if( m_content.size() <= endPos )
                        m_content.resize(endPos+1);

                    for(int i=0; i<bitVal.size() && i<m_content.size(); i++)
                        m_content.setBit(i+startPos, (bool)bitVal.testBit(i));

                    updateRegisterValue(m_content);
                }
                break;

         }
     }

     return true;
 }

 Qt::ItemFlags RegisterFieldModel::flags(const QModelIndex & /*index*/) const
 {
     return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
 }

 QVariant RegisterFieldModel::headerData(int section, Qt::Orientation orientation, int role) const
  {
      if (role == Qt::DisplayRole)
      {
          if (orientation == Qt::Horizontal) {
              switch(section)
              {
                  case ((int)field_map_e::Name):              return QString("Name");
                  case ((int)field_map_e::StartPos):          return QString("StartBit");
                  case ((int)field_map_e::EndPos):            return QString("EndBit");
                  case ((int)field_map_e::ExtractedValue):    return QString("Value");
              }
          }
      }
      return QVariant();
  }

 const QBitArray& RegisterFieldModel::getRegister() const
 {
     return m_content;
 }

 int RegisterFieldModel::getBitArraySize()
 {
     int ret = 0;

     for(field_t& currField: m_bitFields)
     {
         if( !currField.isValidRange() )
             continue;

         if( currField.end > ret )
             ret = currField.end;
     }

     return ret+1;      // size = lastBit + 1
 }

 //TODO: make this functions faster (currently for each row setData is called)
 void RegisterFieldModel::updateRegisterValue(const QBitArray& aContent)
 {
     m_content = aContent;

     qDebug()<<__PRETTY_FUNCTION__;
     qDebug()<<"    "<<aContent;

     for(int iFields=0; iFields<m_bitFields.size(); iFields++)
     {
         if( !m_bitFields[iFields].isValidRange() )
             continue;

         m_bitFields[iFields].extracted = NsConverter::extract(aContent,
                                                               m_bitFields[iFields].start,
                                                               m_bitFields[iFields].end);

         QModelIndex changedIndex1 = this->index(iFields, ((int)field_map_e::StartPos));
         QModelIndex changedIndex2 = this->index(iFields, ((int)field_map_e::ExtractedValue));

         emit dataChanged(changedIndex1, changedIndex2);
         emit fieldChanged(m_content);
     }

     emit fieldChanged(m_content);
 }

 const RegisterFieldModel::fieldlist_t& RegisterFieldModel::getField()
 {
     return m_bitFields;
 }

 //FIXME: make a better update routine
 bool RegisterFieldModel::addRow(QString aName)
 {
     for(const auto iRow: m_bitFields )
     {
         if( iRow.name == aName  )
             return false;
     }

     beginInsertRows(QModelIndex(), m_bitFields.size(), m_bitFields.size());
     m_bitFields.push_back(   field_t(aName, field_t::INVALID_POS, field_t::INVALID_POS, m_colorGen.getNextColor())   );
     endInsertRows();

     updateRegisterValue(m_content);

     return true;
 }

 void RegisterFieldModel::setRegisterName(QString aName)
 {
     m_registerName = aName;
     emit nameChanged(m_registerName);
 }

 bool RegisterFieldModel::removeRows(int row, int count, const QModelIndex & parent)
 {
     if( 0 > row || 1 > count)
         return false;

     beginRemoveRows(parent, row, row+count-1);
     {
        qDebug()<<"remove_row="<<row<<" count="<<count;
        if( 1 == count) {
            //Note: using the 2 paremeter Vector::erase doesn't work
            m_bitFields.erase(m_bitFields.begin()+row);
        }
        else {
            for(int i=row+count-1; i >= row; i--)
                m_bitFields.erase(m_bitFields.begin()+i);
        }
     }
     endRemoveRows();

     updateRegisterValue(m_content);

     return true;
 }

 bool RegisterFieldModel::import(RegisterFieldModel& aRegModel)
 {
     //import bitfield definition
     {
        if( !removeRows(0, rowCount()) )
            return false;

        beginInsertRows(QModelIndex(), 0, aRegModel.rowCount()-1);
        {
            m_bitFields = aRegModel.m_bitFields;
        }
        endInsertRows();

     }

     //import value
     {
         updateRegisterValue(aRegModel.getRegister());
     }

     return true;
 }




