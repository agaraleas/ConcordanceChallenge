#include <gtest/gtest.h>
#include "OutputFormattings.hpp"
#include "Concordance.hpp"

TEST(ConcordanceFormat, IndexConversion)
{
    EXPECT_EQ(makePrintable(1), "a.      ");
    EXPECT_EQ(makePrintable(2), "b.      ");
    EXPECT_EQ(makePrintable(26), "z.      ");
    EXPECT_EQ(makePrintable(27), "aa.     ");
    EXPECT_EQ(makePrintable(28), "bb.     ");
    EXPECT_EQ(makePrintable(29), "cc.     ");
    EXPECT_EQ(makePrintable(52), "zz.     ");
    EXPECT_EQ(makePrintable(53), "aaa.    ");
    EXPECT_EQ(makePrintable(1258796214524), "#######.");
}

TEST(ConcordanceFormat, WordDisplay)
{
    EXPECT_EQ(makePrintable("all"), "all                 ");
    EXPECT_EQ(makePrintable("averydummylaargeword"), "averydummylaargeword");
    EXPECT_EQ(makePrintable("averydummylargewordthatexceedslimit"), "averydummylargewo...");
}

TEST(ConcordanceFormat, OccurrenceDisplay)
{
    Occurrences occurrences;
    occurrences << 1;
    EXPECT_EQ(makePrintable(occurrences), "{1:1}");
    occurrences << 3 << 4;
    EXPECT_EQ(makePrintable(occurrences), "{3:1,3,4}");
}