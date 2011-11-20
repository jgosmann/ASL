#ifndef DOUBLESPINBOXWRAPPER_H
#define DOUBLESPINBOXWRAPPER_H

#include <QDoubleSpinBox>


namespace gui
{

  class DoubleSpinBoxWrapper : public QDoubleSpinBox
  {
    Q_OBJECT

  public:
    DoubleSpinBoxWrapper() : QDoubleSpinBox(NULL)
    {
      connect(this, SIGNAL( valueChanged(double) ),
          this, SLOT( emitValueChanged(double) ));
    }
    ~DoubleSpinBoxWrapper() {}

  public slots:
    void emitValueChanged(double value)
    {
      Q_UNUSED(value)
      emit valueChanged();
    }

  signals:
    void valueChanged();

  };

} // namespace gui

#endif /* DOUBLESPINBOXWRAPPER_H */
