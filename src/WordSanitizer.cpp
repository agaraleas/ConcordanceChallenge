#include "WordSanitizer.hpp"

#include <algorithm>
#include <iterator>

#include "Singleton.hpp"


//INTERNAL CLASS DECLARATIONS
//==========================================================================|
//					WordSanitizer::SymbolHandlingStrategy					|
//==========================================================================|
class WordSanitizer::SymbolHandlingStrategy
{
public:
	SymbolHandlingStrategy(){}
	virtual ~SymbolHandlingStrategy(){}

	virtual void removeUndesiredSymbols(Word &word);
	virtual bool isUndesiredSymbol(char c) const = 0;
};
//END OF INTERNAL CLASS DECLARATIONS



// INTERNAL AUXILIARY CLASSES AND FUNCTIONS
// INTERNAL CLASS DECLARATIONS
namespace
{

//==========================================================================|
//							AllSymbolsRemover								|
//==========================================================================|
class AllSymbolsRemover : public WordSanitizer::SymbolHandlingStrategy,
						  public Singleton<AllSymbolsRemover>
{
public:
	friend class Singleton<AllSymbolsRemover>;

public:
	~AllSymbolsRemover(){}
	bool isUndesiredSymbol(char c) const override;

private:
	AllSymbolsRemover(){}

};
//==========================================================================|
//								DotKeeper									|
//==========================================================================|
class AbbreviationsKeeper : public WordSanitizer::SymbolHandlingStrategy,
				  public Singleton<AbbreviationsKeeper>
{
public:
	friend class Singleton<AbbreviationsKeeper>;

public:
	~AbbreviationsKeeper(){}
	void removeUndesiredSymbols(Word &word) override;
	bool isUndesiredSymbol(char c) const override;

private:
	AbbreviationsKeeper(){}

};

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
	return std::isalnum(c);
}

static bool isDot(char c)
{
	return c == '.';
}

static size_t countTraillingDots(const Word &word)
{
	size_t trailing_dots = 0;
	auto reverse_it = word.rbegin();

	while( reverse_it != word.rend() && isDot(*reverse_it) ){
		++trailing_dots;
		++reverse_it;
	}

	return trailing_dots;
}

static void removeExcessiveDots(Word &word)
{
	size_t trailing_dots = countTraillingDots(word);

	for( size_t i = 1; i < trailing_dots; ++i ){
		word.pop_back();
	}
}

// END OF AUXILIARY FUNCTIONS

// INTERNAL AUXILIARY CLASS DEFINITIONS
bool AllSymbolsRemover::isUndesiredSymbol(char c) const
{
	if( isAlphanumeric(c) ) return false;
	return true;
}

bool AbbreviationsKeeper::isUndesiredSymbol(char c) const
{
	if( isAlphanumeric(c) ) return false;
	if( isDot(c) ) return false;
	return true;
}

void AbbreviationsKeeper::removeUndesiredSymbols(Word &word)
{
	SymbolHandlingStrategy::removeUndesiredSymbols(word);
	removeExcessiveDots(word);
}
// END OF AUXILIARY CLASS DEFINITIONS

}//ANONYMOUS NAMESPACE
// END OF INTERNAL AUXILIARY CLASSES AND FUNCTIONS



//INTERNAL CLASS DEFINITIONS
void WordSanitizer::SymbolHandlingStrategy::removeUndesiredSymbols(Word &word)
{
	auto is_undesired = [this](char c){
		return isUndesiredSymbol(c);
	};

	auto is_desired = [this](char c){
		return !isUndesiredSymbol(c);
	};

	if( std::any_of(word.begin(), word.end(), is_undesired) ){
		Word sanitized;
		std::copy_if(word.begin(), word.end(), std::back_inserter(sanitized), is_desired);
		word = sanitized;
	}
}
//END OF INTERNAL CLASS DEFINITIONS



//EXTERNAL CLASS DEFINITIONS
//==========================================================================|
//								WordSanitizer								|
//==========================================================================|
WordSanitizer::WordSanitizer()
{
	m_symbol_handling = AllSymbolsRemover::instance();
}

void WordSanitizer::setAbbreviationSanitization()
{
	m_symbol_handling = AbbreviationsKeeper::instance();
}

Word WordSanitizer::sanitize(const Word &word)
{
	Word sanitized = word;
	toLowercase(sanitized);
	m_symbol_handling->removeUndesiredSymbols(sanitized);
	return sanitized;
}
//END OF INTERNAL CLASS DEFINITIONS


