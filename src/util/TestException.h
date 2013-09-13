#ifndef UTIL_TESTEXCEPTION_H
#define UTIL_TESTEXCEPTION_H

#include <vector>

#include <util/Exception.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the Exception class.
 *
 * @author Swarming Logic
 */
class TestException : public CxxTest::TestSuite
{
public:
  void testExceptionBasic( void )
  {
    const std::string& msg = "mymessage";
    Exception e(msg);
    TS_ASSERT_EQUALS(e.toString(), msg);

    // Copy
    Exception e2(e);
    TS_ASSERT_EQUALS(e.toString(), msg);

    // Assign construct
    Exception e3 = e;
    TS_ASSERT_EQUALS(e.toString(), msg);

    // What()
    TS_ASSERT_SAME_DATA(e.what(), msg.c_str(), msg.size());

    // What() extended.
    const std::string msgCopy(e.what());
    e = Exception("something else");
    TS_ASSERT_EQUALS(msg, msgCopy);
  }

  void testExceptionThrowCatch( void )
  {
    const std::string& msg = "mymessage";
    bool exceptionCought = false;
    try {
      this->throwException(msg);
    }
    catch (const Exception& e) {
      TS_ASSERT_EQUALS(e.toString(), msg);
      exceptionCought = true;
    }
    TS_ASSERT(exceptionCought);
    TS_ASSERT_THROWS(this->throwException(msg), Exception);
  }


  void testExceptionRaise() {
    const std::string& msg = "mymessage";
    bool exceptionCought = true;

   // Raise
    Exception exception(msg);
    TS_ASSERT_THROWS(exception.raise(), Exception);
    try {
      exception.raise();
    }
    catch (const Exception& e) {
      TS_ASSERT_EQUALS(e.toString(), msg);
      exceptionCought = true;
    }
    TS_ASSERT(exceptionCought);
  }

private:
  void throwException(const std::string& msg) {
    throw Exception(msg);
  }
};

#endif
