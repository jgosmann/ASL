#ifndef DEFAULTASLPROGRAMCOMPILER_H
#define DEFAULTASLPROGRAMCOMPILER_H

#include "aslcompiler.h"
#include "aslprogramcompiler.h"
#include "relativedependencylocator.h"

namespace asl
{
class DefaultASLProgramCompiler : public ASLProgramCompiler
{
public:
    DefaultASLProgramCompiler()
        : ASLProgramCompiler(m_dependencyLocator, m_compiler),
        m_compiler(*this) { }

private:
    ASLCompiler m_compiler;
    RelativeDependencyLocator m_dependencyLocator;
};
} /* namespace asl */

#endif /* DEFAULTASLPROGRAMCOMPILER_H */

