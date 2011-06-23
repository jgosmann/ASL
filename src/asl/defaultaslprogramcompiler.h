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
        : m_compiler(*this), ASLProgramCompiler(m_dependencyLocator, m_compiler) { }

private:
    ASLCompiler m_compiler;
    RelativeDependencyLocator m_dependencyLocator;
};
} /* namespace asl */

#endif /* DEFAULTASLPROGRAMCOMPILER_H */

