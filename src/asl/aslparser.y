
%{
    #ifdef COMPILES_TEST
        #include "../src/asl/aslparser_internal.h"
        #include "../src/asl/shaderparameterinfobuilder.h"
    #else
        #include "asl/aslparser_internal.h"
        #include "asl/shaderparameterinfobuilder.h"
    #endif

    #include <exception>

    #include <GL/gl.h>

    #include <QFileInfo>
    #include <QList>
    #include <QSet>
    #include <QStringBuilder>
    #include <QVector>

    extern int aslparserlineno;
    void yyerror(const char *msg);
    int yylex();

    using namespace asl::parserinternal;

    namespace asl
    {
    namespace parserinternal
    {
        QString log;
        unsigned int sourceStringNo;
        bool parsedFirstAslComment;
        ShaderInfo shaderInfo;
        ShaderParameterInfoBuilder parameterInfoBuilder;

        QHash<QString, unsigned int> definedKeys;

        const QString UNKNOWN_KEY_WARNING("unknown key: ");

        void addToLog(const QString &type, const QString &msg);
        void handleKeyStringValuePair(const QString &key, const QString &value,
            unsigned int argNumber = 0);
        void handleKeyGLVariantValuePair(const QString &key,
            const GLVariant &value, unsigned int argNumber = 0);
        template<class T> GLVariant * glvariantFromValueList(
            const GLTypeInfo &type, const QList<GLVariant *> *values);
        template<class CastToT> QVector<CastToT> glvariantListToCastedQVector(
            const QList<GLVariant *> *values);
        template<class T> GLVariant * savelyCreateGLVariant(
            const GLTypeInfo &type, GLsizei count, const T *value);
        void warnIfKeyDefinedAndDefineKey(const QString &key,
            unsigned int argNumber);
        void warn(const QString &msg);
    } /* namespace parserinternal */
    } /* namespace asl */

%}

%union {
    unsigned long int integer;
    GLfloat flt;
    QString *string;
    asl::GLVariant *glvariant;
    QList<asl::GLVariant *> *glvariantList;
}

%type <string> string
%type <glvariant> glvariant
%type <glvariantList> glvariantList

%destructor { delete $$; } string

%token <integer> INTEGER BOOLEAN
%token <flt> FLT
%token <string> KEY IDENTIFIER ANNOTATION_STRING
%token ANNOTATION_START ANNOTATION_END UNIFORM LINE END NEGATE UNEXPECTED_CHAR
%token '(' ')' ','

%%

program:
    leadingChars
    | pplines aslStartComment remainingProgram
    | leadingChars aslStartComment remainingProgram {
            warn("ASL program is not starting with ASL comment.");
        }
    | ;

aslStartComment: annotationComment {
        asl::parserinternal::parsedFirstAslComment = true;
    }

leadingChars:
    leadingChars UNEXPECTED_CHAR pplines
    | UNEXPECTED_CHAR pplines;

pplines: pplines ppline | ;

remainingProgram:
    remainingProgram parameter
    | remainingProgram ppline
    | ;

ppline:
    LINE INTEGER END {
            aslparserlineno = $2 + 1;
        }
    | LINE INTEGER INTEGER END {
            aslparserlineno = $2 + 1;
            sourceStringNo = $3;
        }

parameter: annotationComment UNIFORM datatype IDENTIFIER parameterEnd {
            parameterInfoBuilder.withIdentifier(*$4);
            try {
                shaderInfo.parameters.append(parameterInfoBuilder.build());
            } catch (const common::NoValueException &e) {
                warn(e.what());
            }
            delete $4;
        }
    | annotationComment error {
            warn("ASL comment is not preceding valid uniform declaration.");
        }

parameterEnd: ';' | '=';

datatype: IDENTIFIER {
            try {
                parameterInfoBuilder.withType(asl::GLTypeInfo::getFor(*$1));
            } catch (const std::invalid_argument &e) {
                warn(e.what());
            }
            delete $1;
        };

annotationComment: ANNOTATION_START {
            definedKeys.clear();
            parameterInfoBuilder.reset();
         } annotations ANNOTATION_END {}

annotations:
    annotations keyValuePair
    | ;

keyValuePair:
    KEY string {
            handleKeyStringValuePair(*$1, *$2);
            delete $1;
            delete $2;
        }
    | KEY IDENTIFIER {
            handleKeyStringValuePair(*$1, *$2);
            delete $1;
            delete $2;
        }
    | KEY IDENTIFIER ',' IDENTIFIER {
            handleKeyStringValuePair(*$1, *$2, 1);
            handleKeyStringValuePair(*$1, *$4, 2);
            delete $1;
            delete $2;
        }
    | KEY glvariant ',' IDENTIFIER {
            handleKeyGLVariantValuePair(*$1, *$2, 1);
            handleKeyStringValuePair(*$1, *$4, 2);
            delete $1;
            delete $2;
        }
    | KEY IDENTIFIER ',' glvariant {
            handleKeyStringValuePair(*$1, *$2, 1);
            handleKeyGLVariantValuePair(*$1, *$4, 2);
            delete $1;
            delete $2;
        }
    | KEY glvariant {
            handleKeyGLVariantValuePair(*$1, *$2);
            delete $1;
            delete $2;
        }
    | KEY glvariant ',' glvariant {
            handleKeyGLVariantValuePair(*$1, *$2, 1);
            handleKeyGLVariantValuePair(*$1, *$4, 2);
            delete $1;
            delete $2;
            delete $4;
        }
    ;

string:
    string ANNOTATION_STRING {
            $$ = $1;
            $$->append(" ");
            $$->append($2);
            delete $2;
        }
    | ANNOTATION_STRING { $$ = $1; }
    ;

glvariant:
    IDENTIFIER '(' glvariantList ')' {
            try {
                const asl::GLTypeInfo &type = asl::GLTypeInfo::getFor(*$1);

                switch (type.type()) {
                    case asl::GLTypeInfo::FLOAT:
                        $$ = glvariantFromValueList<GLfloat>(type, $3);
                        break;
                    case asl::GLTypeInfo::BOOL: /* fall through */
                    case asl::GLTypeInfo::INT:
                        $$ = glvariantFromValueList<GLint>(type, $3);
                        break;
                    case asl::GLTypeInfo::UINT:
                        $$ = glvariantFromValueList<GLuint>(type, $3);
                        break;
                    default:
                        warn("Type not supported.");
                        break;
                }
            } catch (const std::exception &e) {
                warn("Could not instantiate GLSL type: " + *$1);
                warn(e.what());
                $$ = new asl::GLVariant();
            }

            delete $1;
            foreach (asl::GLVariant *value, *$3) {
                delete value;
            }
            delete $3;
        }
    | INTEGER {
            const GLuint value = $1;
            $$ = new asl::GLVariant("uint", 1, &value);
        }
    | NEGATE INTEGER {
            const GLint value = -$2;
            $$ = new asl::GLVariant("uint", 1, &value);
        }
    | FLT {
            const GLfloat value = $1;
            $$ = new asl::GLVariant("float", 1, &value);
        }
    | NEGATE FLT {
            const GLfloat value = -$2;
            $$ = new asl::GLVariant("float", 1, &value);
        }
    | BOOLEAN {
            const GLint value = $1;
            $$ = new asl::GLVariant("bool", 1, &value);
        }
    ;

glvariantList:
    glvariantList ',' glvariant {
            $$ = $1;
            $$->append($3);
        }
    | glvariant {
            $$ = new QList<asl::GLVariant *>();
            $$->append($1);
        }
    ;

%%

using namespace std;
using namespace asl::parserinternal;

void yyerror(const char *msg)
{
    warn(msg);
}

namespace asl
{
namespace parserinternal
{

void addToLog(const QString &type, const QString &msg)
{
    log = log % type % ": " % QString::number(sourceStringNo) % QChar(':')
            % QString::number(aslparserlineno)
            % ": (aslcompiler) " % QString(msg) % QChar('\n');
}

void clearLog()
{
    log.clear();
}

void handleKeyStringValuePair(const QString &key, const QString &value,
        unsigned int argNumber)
{
    warnIfKeyDefinedAndDefineKey(key, argNumber);

    if (parsedFirstAslComment) {
        if ("Name" == key) {
            parameterInfoBuilder.withName(value);

        } else if ("Description" == key) {
            parameterInfoBuilder.withDescription(value);

        } else if ("Range" == key && argNumber == 0) {
            GLuint min;
            if ("percent" == value || "byte" == value || "unsigned" == value) {
                min = 0;
            } else if ("positive" == value) {
                min = 1;
            } else {
                warn("Invalid value: " + value);
                return;
            }
            parameterInfoBuilder.withMinimum(GLVariant("uint", 1, &min));
            if ("percent" == value) {
                GLuint value = 1;
                parameterInfoBuilder.withMaximum(GLVariant("uint", 1, &value));
            } else if("byte" == value) {
                GLuint value = 255;
                parameterInfoBuilder.withMaximum(GLVariant("uint", 1, &value));
            } else if("unsigned" == value || "positive" == value) {
                parameterInfoBuilder.withNoMaximum();
            }

        } else if ("Range" == key && argNumber == 1) {
            parameterInfoBuilder.withNoMinimum();

        } else if ("Range" == key && argNumber == 2) {
            parameterInfoBuilder.withNoMaximum();

        } else if ("Control" == key) {
            parameterInfoBuilder.withPreferredUIControls(
                value.split(QRegExp("\\s*,\\s*")));

        } else {
            warn(UNKNOWN_KEY_WARNING + key);
        }

    } else {
        if ("ShaderName" == key) {
            shaderInfo.name = value;
        } else if ("ShaderDescription" == key) {
            shaderInfo.description = value;
        } else if ("Depends" == key) {
            shaderInfo.dependencies = value.split(QRegExp("\\s*,\\s*"));
        } else {
            warn(UNKNOWN_KEY_WARNING + key);
        }

    }
}

void handleKeyGLVariantValuePair(const QString &key, const GLVariant &value,
        unsigned int argNumber)
{
    warnIfKeyDefinedAndDefineKey(key, argNumber);

    if (!parsedFirstAslComment) {
        warn(UNKNOWN_KEY_WARNING + key);
        return;
    }

    if ("Default" == key) {
        parameterInfoBuilder.withDefaultValue(value);
    } else if ("Range" == key && argNumber == 1) {
        parameterInfoBuilder.withMinimum(value);
    } else if ("Range" == key && argNumber == 2) {
        parameterInfoBuilder.withMaximum(value);
    } else {
        warn(UNKNOWN_KEY_WARNING + key);
    }
}

void warnIfKeyDefinedAndDefineKey(const QString &key, unsigned int argNumber)
{
    if (definedKeys.contains(key)) {
        const bool isProcessingAnUnprocessedArgument =
                definedKeys[key] != 0 && definedKeys[key] < argNumber;
        if (!isProcessingAnUnprocessedArgument) {
            --aslparserlineno;
            warn("duplicate key: " + key);
            ++aslparserlineno;
        }
    } else {
        definedKeys.insert(key, argNumber);
    }
}

template<class T> GLVariant * glvariantFromValueList(const GLTypeInfo &type,
        const QList<GLVariant *> *values)
{
    QVector<T> castedValues = glvariantListToCastedQVector<T>(values);
    return savelyCreateGLVariant(type, castedValues.size(),
            castedValues.constData());
}

template<class CastToT> QVector<CastToT> glvariantListToCastedQVector(
        const QList<GLVariant *> *values)
{
    QVector<CastToT> castedValues;
    foreach (GLVariant *value, *values) {
        for (GLsizei i = 0; i < value->count(); ++i) {
            castedValues.append(value->ithValueCasted<CastToT>(i));
        }
    }

    return castedValues;
}

template<class T> GLVariant * savelyCreateGLVariant(const GLTypeInfo &type,
        GLsizei count, const T *value)
{
    try {
        return new GLVariant(type, count, value);
    } catch (const exception &e) {
        warn("Could not instantiate GLSL type: " + type.glslName());
        warn(e.what());
        return new GLVariant();
    }
}

const ShaderInfo parse(const QString &sourcecode,
        const QString &pathOfSource)
{
    shaderInfo = ShaderInfo();
    shaderInfo.name = QFileInfo(pathOfSource).fileName();
    parsedFirstAslComment = false;
    setInput(sourcecode);
    yyparse();
    return shaderInfo;
}

void warn(const QString &msg)
{
    addToLog("WARNING", msg);
}

} /* namespace parserinternal */
} /* namespace asl */

