#ifndef __UNIFORMOPTIONBOX_H
#define __UNIFORMOPTIONBOX_H

#include <QGroupBox>

namespace gui {

  typedef asl::GLTypeInfo::Structure UniformStructure;
  typedef asl::GLTypeInfo::Type UniformType;

  class UniformOptionBox : public QGroupBox
  {
    Q_OBJECT

    public:
      UniformOptionBox(QWidget* parent = NULL);
      virtual ~UniformOptionBox();

    signals:
      /**
       *  This signal directly transmits all information to GLImageViewer that
       *  it requires to execute "setUniform" on his shaderProgram.
       */
      void uniformChanged(QString& glslName, void* value);
  };
}

#endif // __UNIFORMOPTIONBOX_H
