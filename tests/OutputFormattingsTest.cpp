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

TEST(ConcordanceFormat, ConcordanceJoin)
{
    WordIndex index = 3;
    Word word = "angelo";
    Occurrences occurrences; occurrences << 1;
    EXPECT_EQ(joinConcordanceLine(index, word, occurrences), "c.       angelo               {1:1}");

    index = 99999999999;
    word = "longlonglonglonglonglonglonglongword";
    occurrences = Occurrences(); occurrences << 1 << 2 << 3 << 4;
    EXPECT_EQ(joinConcordanceLine(index, word, occurrences), "#######. longlonglonglongl... {4:1,2,3,4}");

    index = 4;
    word = "longlonglonglonglonglonglonglongword";
    occurrences = Occurrences(); occurrences << 1 << 2 << 3 << 4;
    EXPECT_EQ(joinConcordanceLine(index, word, occurrences), "d.       longlonglonglongl... {4:1,2,3,4}");

    index = 33333333333;
    word = "angelo";
    EXPECT_EQ(joinConcordanceLine(index, word, occurrences), "#######. angelo               {4:1,2,3,4}");
}