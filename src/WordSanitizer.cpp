#include "WordSanitizer.hpp"
#include <algorithm>

// INTERNAL AUXILIARY CLASSES AND FUNCTIONS
namespace
{

// AUXILIARY FUNCTIONS
static void toLowercase(Word &word)
{
	auto convert_to_lowercase = [](char c){
		return std::tolower(c);
	};

	std::transform(word.begin(), word.end(), word.begin(), convert_to_lowercase);
}

static bool isAlphanumeric(char c)
{
	return std::isalnum(static_cast<unsigned char>(c));
}
// END OF AUXILIARY FUNCTIONS

}//ANONYMOUS NAMESPACE
// END OF INTERNAL AUXILIARY CLASSES AND FUNCTIONS



//EXTERNAL CLASS DEFINITIONS
//==========================================================================|
//								WordSanitizer								|
//==========================================================================|
Word WordSanitizer::sanitize(const Word &word)
{
	Word sanitized = word;
	toLowercase(sanitized);
	return sanitized;
}
//END OF INTERNAL CLASS DEFINITIONS


