#ifndef SPINBOXWRAPPER_H
#define SPINBOXWRAPPER_H

#include <QSpinBox>


namespace gui
{

  class SpinBoxWrapper : public QSpinBox
  {
    Q_OBJECT

  public:
    SpinBoxWrapper() : QSpinBox(NULL)
    {
      connect(this, SIGNAL( valueChanged(int) ),
          this, SLOT( emitValueChanged(int) ));
    }
    ~SpinBoxWrapper() {}

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

#endif /* SPINBOXWRAPPER_H */
