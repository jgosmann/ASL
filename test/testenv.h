
#ifndef COMMON_H
#define COMMON_H

#include <cppunit/TestFixture.h>
#include <cppunit/Message.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gmock/gmock.h>

#include <iostream>

#include <QString>

using namespace CppUnit;

using namespace testing;

std::ostream & operator<<(std::ostream &output, const QString &str);

#endif

