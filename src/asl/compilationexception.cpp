#include "compilationexception.h"

using namespace asl;

CompilationException::CompilationException(Stage stage, const QString &log)
    : m_stage(stage), m_line(0), m_log(log)
{
}

CompilationException::CompilationException(Stage stage, const QString &msg,
        unsigned int line)
    : m_stage(stage), m_line(line), m_log(QString::number(line) + ": " + msg)
{
}

