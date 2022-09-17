#include "WordValidator.hpp"

#include <algorithm>

//INTERNAL AUXILIARY CLASSES AND FUNCTIONS
static bool shouldBeIgnoredInValidations(char c)
{
	return c == '.';
}

static bool containsIllegalSymbols(const Word &word)
{
	auto is_illegal = [](char c){
		return !std::isalnum(c) && !shouldBeIgnoredInValidations(c);
	};

	return std::any_of(word.begin(), word.end(), is_illegal);
}

static bool areAlphabeticalCharactersMissing(const Word &word)
{
	return std::none_of(word.begin(), word.end(), std::isalpha);
}
//END OF INTERNAL AUXILIIARY CLASSES AND FUNCTIONS



//EXTERNAL CLASS DEFINITIONS
bool WordValidator::isValid(const Word &word)
{
	if( word.empty() ){
		return false;
	}

	if( containsIllegalSymbols(word) ){
		return false;
	}

	if( areAlphabeticalCharactersMissing(word) ){
		return false;
	}

	return true;
}
//END OF EXTERNAL CLASS DEFINITIONS
