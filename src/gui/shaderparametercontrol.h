#ifndef SHADERPARAMETERCONTROL_H
#define SHADERPARAMETERCONTROL_H

#include <QGLShaderProgram>
#include <QWidget>

#include "../asl/shaderparameterinfo.h"

namespace gui
{
template<class ControlT, class ParamT> class ShaderParameterControl
        : public QWidget
{
    Q_OBJECT

public:
    // 1. m_controls als Array mit genug Elementen initialisieren
    // 2. Zeilen/Spalten durchgehen, Controls erstellen, Min, Max und default
    //    wert setzen und jeweils connectSingleControlSignal() aufrufen.
    ShaderParameterControl(const ShaderParameterInfo &type,
            QGLShaderProgram &prgm);

    // Speicher von m_controls freigeben
    virtual ~ShaderParameterControl();

private slots:
    // Array mit genug Elementen vom Typ ParamT anlegen (sicherstellen das
    // dieses beim Verlassen der Funktion wieder freigegeben wird)
    // m_controls durchgehen und mit getSingelValueFromControl() Wert auslesen
    // und in das Array schreiben. Uniform von m_prgm setzen.
    void setParameterFromControls();

private:
    // Liest den aktuellen Wert von control aus und gibt ihn zurück.
    ParamT getSingleValueFromControl(const ControlT &control);

    // "valueChanged" signal von control mit setParaemterFromControlsVerbinden()
    void connectSingleControlSignal(const ControlT &control);

    ControlT *m_controls;
    QGLShaderProgram m_prgm;
};
}

#endif /* SHADERPARAMETERCONTROL_H */

