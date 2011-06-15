#ifndef SHADERLISTMODEL_H
#define SHADERLISTMODEL_H

#include <QAbstractListModel>
#include <QSharedPointer>
#include <QList>
#include <asl/annotatedglshaderprogram.h>

typedef class asl::AnnotatedGLShaderProgram Shader;

class ShaderListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ShaderListModel(QObject *parent = 0);
    int rowCount( const QModelIndex & parent = QModelIndex() ) const;
    QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /**
      * Adds a shader to the List
      */
    void addShader(QSharedPointer<Shader> shader);

private:
    QList< QSharedPointer<Shader> > shaderList;

signals:

public slots:

};

#endif // SHADERLISTMODEL_H
