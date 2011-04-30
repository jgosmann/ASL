#ifndef COMPILATIONEXCEPTION_H
#define COMPILATIONEXCEPTION_H

#include <exception>
#include <QString>

namespace asl
{
    class CompilationException : public std::exception
    {
    public:
        enum Stage {
            PREPROCESSING,
            COMPILATION,
            LINKING
        };

        CompilationException(Stage stage, const QString &log);
        ~CompilationException() throw() { }

        inline Stage stage() const throw() {
            return m_stage;
        }

        inline const QString & log() const throw() {
            return m_log;
        }

        inline const char * what() const throw() {
            return log().toAscii();
        }

    private:
        const Stage m_stage;
        const QString m_log;
    };
}

#endif // COMPILATIONEXCEPTION_H
