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
      connect(this, SIGNAL( valueChanged(int) ),
          this, SLOT( emitValueChanged(int) ));
    }
    ~DoubleSpinBoxWrapper() {}

  public slots:
    void emitValueChanged(int value)
    {
      Q_UNUSED(value)
      emit valueChanged();
    }

  signals:
    void valueChanged();

  };

} // namespace gui

#endif /* DOUBLESPINBOXWRAPPER_H */
