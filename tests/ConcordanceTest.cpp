#include <gtest/gtest.h>
#include "Concordance.hpp"

TEST(ConcordanceTests, DummyTest)
{
    const std::string expected = "Hello World";
    const std::string actual = getHelloMessage();
    ASSERT_EQ(expected, actual);
}