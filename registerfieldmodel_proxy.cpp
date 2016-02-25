#include "registerfieldmodel_proxy.h"

Registerfieldmodel_proxy::Registerfieldmodel_proxy(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

bool Registerfieldmodel_proxy::lessThan(const QModelIndex &left,
                                        const QModelIndex &right) const
{
    QVariant leftData   = sourceModel()->data(left);
    QVariant rightData  = sourceModel()->data(right);

    if( QVariant::String == leftData.type() )
    {
        return leftData.toString() < rightData.toString();
    }
    else if( QVariant::Int == leftData.type() )
    {
        return leftData.toInt() < rightData.toInt();
    }

    return false;
}
