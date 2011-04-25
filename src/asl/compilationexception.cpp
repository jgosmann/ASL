#include "compilationexception.h"

using namespace asl;

CompilationException::CompilationException(Stage stage, const QString &log)
    : m_stage(stage), m_log(log)
{
}
