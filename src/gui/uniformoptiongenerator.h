#ifndef __UNIFORMOPTIONGENERATOR_H
#define __UNIFORMOPTIONGENERATOR_H

#include <QWidget>
#include <QScrollArea>

namespace gui {

  class UniformOptionGenerator : public QScrollArea
  {
    Q_OBJECT

  public:
    UniformOptionGenerator(QWidget*);
    virtual ~UniformOptionGenerator();
  };
}

#endif // __UNIFORMOPTIONGENERATOR_H
