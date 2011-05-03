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
        CompilationException(Stage stage, const QString &msg,
            unsigned int line);
        ~CompilationException() throw() { }

        inline Stage stage() const throw() {
            return m_stage;
        }

        inline unsigned int line() const throw() {
            return m_line;
        }

        inline const QString & log() const throw() {
            return m_log;
        }

        inline const char * what() const throw() {
            return log().toAscii();
        }

    private:
        const Stage m_stage;
        const unsigned int m_line;
        const QString m_log;
    };
}

#endif // COMPILATIONEXCEPTION_H
