#ifndef REGISTERFIELDMODEL_PROXY_H
#define REGISTERFIELDMODEL_PROXY_H

/*
 *  mabe this class could be deleted because the base class also provide this function
 */

#include <QSortFilterProxyModel>

class Registerfieldmodel_proxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit Registerfieldmodel_proxy(QObject *parent = 0);
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

signals:

public slots:

};

#endif // REGISTERFIELDMODEL_PROXY_H
