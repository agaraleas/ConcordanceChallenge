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

TEST(WordSanitization, SymbolExtraction)
{
    WordSanitizer sanitizer;
    EXPECT_EQ("hey", sanitizer.sanitize("hey!"));
    EXPECT_EQ("really", sanitizer.sanitize("REALLY?"));
    EXPECT_EQ("finally", sanitizer.sanitize("finally."));
    EXPECT_EQ("meaning", sanitizer.sanitize("meaning..."));
    EXPECT_EQ("whitespace", sanitizer.sanitize("whitespace \n"));
    EXPECT_EQ("like", sanitizer.sanitize("like:"));
    EXPECT_EQ("stop", sanitizer.sanitize("stop;"));
    EXPECT_EQ("exclamation", sanitizer.sanitize("EXCLAMATION!!!!!!!"));
    EXPECT_EQ("joinwithunderscore", sanitizer.sanitize("join_with_underscore")); //not really a use case but anyway
    EXPECT_EQ("comma", sanitizer.sanitize(",comma"));
    EXPECT_EQ("sort", sanitizer.sanitize("~sort"));
    EXPECT_EQ("quotes", sanitizer.sanitize("'Quotes'"));
    EXPECT_EQ("parenthesis", sanitizer.sanitize("(PARENTHESIS)"));
    EXPECT_EQ("brackets", sanitizer.sanitize("[brackets]"));
    EXPECT_EQ("anglebrackets", sanitizer.sanitize("{anglebrackets}"));
}

TEST(WordSanitization, DotHandling)
{
    WordSanitizer sanitizer;
    sanitizer.setAbbreviationSanitization();
    EXPECT_EQ("i.e.", sanitizer.sanitize("I.e."));
    EXPECT_EQ("a.k.a.", sanitizer.sanitize("a.k.a."));
    EXPECT_EQ("t.b.c", sanitizer.sanitize("T.B.C"));
}

TEST(WordSanitization, NumberAcronyms)
{
    WordSanitizer sanitizer;
    EXPECT_EQ("c4i", sanitizer.sanitize("C4I"));
    EXPECT_EQ("i2i", sanitizer.sanitize("I2I"));
    EXPECT_EQ("b2b", sanitizer.sanitize("B.2.B."));
    
    sanitizer.setAbbreviationSanitization();
    EXPECT_EQ("b.2.b.", sanitizer.sanitize("B.2.B."));
    EXPECT_EQ("a.g.", sanitizer.sanitize("a.g.."));
}