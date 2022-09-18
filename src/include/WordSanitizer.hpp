#ifndef WORDSANITIZER_HPP
#define WORDSANITIZER_HPP

//Include Headers
#include <string>

//Typedefs
using Word = std::string;

//==========================================================================|
//								WordSanitizer								|
//==========================================================================|
// @brief: Sanitizes a word to be used in a concordance.					|
//		   Sanitization is necessary since words of concordance should		|
//         be lowercase														|
//==========================================================================|
class WordSanitizer
{
public:
	static Word sanitize(const Word &word);
};

#endif