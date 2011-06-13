#ifndef SHADERITEMLIST_H
#define SHADERITEMLIST_H

#include <QStandardItemModel>

/// The model-class of the shaderlist
class ShaderItemList : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ShaderItemList(QObject *parent = 0);
    /// Implements the flags-method for setting the dropping functions.
    Qt::ItemFlags flags ( const QModelIndex & index ) const;

signals:

public slots:

};

#endif // SHADERITEMLIST_H
