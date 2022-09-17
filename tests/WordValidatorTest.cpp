#include <gtest/gtest.h>
#include "WordValidator.hpp"

TEST(WordValidations, Validations)
{
	WordValidator validator;
	EXPECT_FALSE(validator.isValid(""));
	EXPECT_TRUE(validator.isValid("hello"));
	EXPECT_FALSE(validator.isValid("multiple words"));
	EXPECT_FALSE(validator.isValid("b@11sh1t"));
	EXPECT_TRUE(validator.isValid("a.k.a"));
	EXPECT_TRUE(validator.isValid("B2B"));
	EXPECT_FALSE(validator.isValid("0623141258"));
	EXPECT_FALSE(validator.isValid("06.23.14.12.58"));
}