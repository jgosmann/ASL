
#ifndef COMMON_H
#define COMMON_H

#include <cppunit/TestFixture.h>
#include <cppunit/Message.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gmock/gmock.h>

#include <iostream>

#include <QString>
#include <QStringList>

using namespace CppUnit;

using namespace testing;

std::ostream & operator<<(std::ostream &output, const QString &str);
template<class T> std::ostream & operator<<(std::ostream &output,
        const QList<T> &list)
{
    bool first = true;
    foreach (T elem, list) {
        if (!first) {
            output << ", ";
        }
        output << elem;
    }

    return output;
}

#endif

