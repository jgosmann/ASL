#ifndef WIDGETWRAPPER_H
#define WIDGETWRAPPER_H

#include <assert.h>

#include <QObject>
#include <QHash>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>
#include <QSlider>

#include "../asl/shaderparameterinfo.h"
#include "../asl/gltypeinfo.h"
#include "../asl/glvariant.h"

namespace gui
{

  class WidgetWrapper : public QObject
  {
    Q_OBJECT

  public:
    /**
     * This is the default constructor that is called, when WidgetWrapper shall
     * create several single widgets for one control.
     */
    WidgetWrapper(asl::ShaderParameterInfo &info);

    /**
     * This constructor is an alternative to the default constructor that
     * permits the creation of a single widget that is created from a template
     * and is related to a whole uniform.
     */
    /*WidgetWrapper(Preset &preset);*/

    ~WidgetWrapper();

    /**
     * This enum helps to differ from preset-widgets the user proposed in ASL 
     * using the key "Control" that refers to a widget-type that shall control
     * the parameter.
     */
    enum Preset 
    {
      COLOR,
      SLIDER,
      SPINBOX,
      DEFAULT
    };

    /**
     * Update the iterator for example when one instance has been erased from
     * the hashmap and all other iterators have become invalid.
     */
    void setIterator( 
        QHash<asl::ShaderParameterInfo, WidgetWrapper*>::iterator iter ) 
    { 
      m_iter = iter; 
    }

    QWidget& widget() const 
    { 
      return *m_widget;
    }

    const asl::GLVariant& value() const 
    { 
      return m_value; 
    }

    void setValue(asl::GLVariant &value) 
    { 
      m_value = value; 
    }

    void setRange(asl::GLVariant &min, asl::GLVariant &max);

  signals:
    template<class ParamT> void updateParameter(ParamT* values);

  private:
    static QHash<asl::ShaderParameterInfo, WidgetWrapper*> m_instances_;

    /**
     * This is a list of all supported presets of control elements.
     */
    static QStringList m_supportedUIControls_;
    /**
    * This function is called when a signal reached the changeValue()-slot.
    */
    template<class ParamT> void update();

    /**
     * This function creates a WidgetWrapper dependent to preset and registers
     * it as related to m_info.
     */
    void createWidgetWrapperFromPreset(Preset &preset, QVBoxLayout *vBoxLayout,
        WidgetWrapper *widgetWrapper );

    /**
     * In the destructor each instance of WidgetWrapper will find itself in
     * m_instances with help of this iterator.
     */
    QHash<asl::ShaderParameterInfo, WidgetWrapper*>::iterator m_iter;

    /**
     * This is a placeholder for the "real" QWidget.
     */
    QWidget *m_widget;

    /**
     * This is a placeholder for the ShaderParameterInfo.
     */
    asl::ShaderParameterInfo m_info;

    /**
     * This is a placeholder for the value as a multitype GLVariant.
     */
    asl::GLVariant m_value;

    /**
     * This is a placeholder for the preset type. It is needed for distinction
     * in constructor and value().
     */
    Preset m_preset;

  private slots:
    /**
     * This slot opens a QColorDialog and writes the selected color to m_value.
     */
    void showColorDialog();

    /**
     * This slot is a template function because it shall receive different
     * types of "valueChanged(..)" signals. It collects all WidgetWrapper
     * instances that are related to m_info and emits their values in an array.
     */
    template<class ParamT> void changeValue(ParamT value);
  };


  // template-class implementations:

  template<class ParamT> void WidgetWrapper::update()
  {
    QList<WidgetWrapper> controls = m_instances_.values( m_info );

    ParamT *values = new ParamT[ controls.length() ];

    unsigned short int i = 0;
    foreach( WidgetWrapper control, controls )
    {
      values[ i++ ] = control.value().as<ParamT>();
      
      emit updateParameter( values );
    }
  }

  template<class ParamT> void changeValue(ParamT value)
  {
    m_value = asl::GLVariant<ParamT>( m_info, 1, &value );

    update<ParamT>();
  }

} // namespace gui

#endif /* WIDGETWRAPPER_H */
