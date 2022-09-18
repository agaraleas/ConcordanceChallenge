#include <gtest/gtest.h>
#include "WordSanitizer.hpp"

TEST(WordSanitization, Capitalization)
{
    WordSanitizer sanitizer;
    EXPECT_EQ("lowercaseall", sanitizer.sanitize("lowercaseall"));
    EXPECT_EQ("uppercaseall", sanitizer.sanitize("UPPERCASEALL"));
    EXPECT_EQ("uppercasefirst", sanitizer.sanitize("Uppercasefirst"));
    EXPECT_EQ("uppercaserandom", sanitizer.sanitize("UppercaseRandoM"));
}

TEST(WordSanitization, DotHandling)
{
    WordSanitizer sanitizer;
    EXPECT_EQ("i.e.", sanitizer.sanitize("I.e."));
    EXPECT_EQ("a.k.a.", sanitizer.sanitize("a.k.a."));
    EXPECT_EQ("t.b.c", sanitizer.sanitize("T.B.C"));
}

TEST(WordSanitization, NumberAcronyms)
{
    WordSanitizer sanitizer;
    EXPECT_EQ("c4i", sanitizer.sanitize("C4I"));
    EXPECT_EQ("i2i", sanitizer.sanitize("I2I"));
    EXPECT_EQ("b.2.b.", sanitizer.sanitize("B.2.B."));
    EXPECT_EQ("a.g.", sanitizer.sanitize("a.g."));
}