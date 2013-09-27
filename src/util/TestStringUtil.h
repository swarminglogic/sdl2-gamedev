#ifndef UTIL_TESTSTRINGUTIL_H
#define UTIL_TESTSTRINGUTIL_H

#include <string>
#include <vector>

#include <util/StringUtil.h>

#include <cxxtest/TestSuite.h>


/**
 * Test for the StringUtil class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TestStringUtil : public CxxTest::TestSuite
{
public:
  void testSplit()
  {
    std::string txt {"Hello my \n   sweet world!"};
    std::vector<std::string> res = StringUtil::split(txt, '\n');
    TS_ASSERT(!res.empty());
    TS_ASSERT_EQUALS(res.size(), 2);
    TS_ASSERT_EQUALS(res[0], "Hello my ");
    TS_ASSERT_EQUALS(res[1], "   sweet world!");
  }

  void testTrim()
  {
    std::string txt {"   Hello world!   "};
    TS_ASSERT_EQUALS(StringUtil::ltrimc(txt), "Hello world!   ");
    TS_ASSERT_EQUALS(StringUtil::rtrimc(txt), "   Hello world!");
    TS_ASSERT_EQUALS(StringUtil::trimc(txt), "Hello world!");

    std::string rt {"   Hello world!   "};
    StringUtil::rtrim(rt);
    TS_ASSERT_EQUALS(rt, "   Hello world!");
    std::string lt {"   Hello world!   "};
    StringUtil::ltrim(lt);
    TS_ASSERT_EQUALS(lt, "Hello world!   ");
    std::string ct {"   Hello world!   "};
    StringUtil::trim(ct);
    TS_ASSERT_EQUALS(ct, "Hello world!");
  }


private:
};

#endif
