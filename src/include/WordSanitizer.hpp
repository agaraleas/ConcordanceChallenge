#ifndef WORDSANITIZER_HPP
#define WORDSANITIZER_HPP

//Include Headers
#include <string>

//Typedefs
using Word = std::string;

//==========================================================================|
//								WordSanitizer								|
//==========================================================================|
// @brief: Sanitizes a word to be used in a concordance						|
//		   sanitization is necessary to get rid of symbols and capital		|
//		   letters															|
//==========================================================================|
class WordSanitizer
{
public:
	WordSanitizer();

	void setAbbreviationSanitization();
	Word sanitize(const Word &word);

public:
	class SymbolHandlingStrategy;

private:
	SymbolHandlingStrategy *m_symbol_handling;
};

#endif