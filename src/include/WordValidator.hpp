#ifndef WORDVALIDATOR_HPP
#define WORDVALIDATOR_HPP

#include <string>
using Word = std::string;

class WordValidator
{
public:
	static bool isValid(const Word &word);
};

#endif