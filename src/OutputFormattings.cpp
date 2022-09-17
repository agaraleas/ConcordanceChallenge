#include "OutputFormattings.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include "Concordance.hpp"

static constexpr size_t MaximumPrintableWordSize = 20; //Measured from example
static constexpr size_t MaximumPrintableIndexSize = 8; //Measured from example

//INTERNAL FUNCTION DEFINITIONS
static void truncate(Word &word)
{
	auto last = word.rbegin();
	*(last++) = '.';
	*(last++) = '.';
	*(last++) = '.';
}

static void fillWithTrailingWhitespace(Word &word, size_t quantity)
{
	std::stringstream ss;
	ss << word;

	for( int i = 0; i < quantity; i++ ){
		ss << " ";
	}
	word = ss.str();
}

static void printWordOfConcordance(WordIndex index, const Word &word, const Occurrences &occurrences)
{
	std::cout << makePrintable(index) << makePrintable(word) << makePrintable(occurrences) << std::endl;
}

static std::tuple<char, size_t> convertWordIndexToEnglishLetters(WordIndex index)
{
	//Convert to zero-based start: First index of Word Index is 1 while C++ 
	// conventions usually assume that first index is 0;
	--index;

	constexpr char first_ascii_letter = 97;
	char letter = first_ascii_letter + index % 26;
	size_t alphabet_cycles = index / 26;
	return std::make_tuple(letter, alphabet_cycles + 1);
}

static std::string joinLettersOfConvertedIndex(char letter, size_t quantity)
{
	std::string joined;
	joined.reserve(quantity);

	std::stringstream ss;

	for( size_t i = 0; i < quantity; i++ ){
		ss << letter;
	}

	return ss.str();
}
//END OF INTERNAL FUNCTION DEFINITIONS


//EXTERNAL FUNCTION DEFINITIONS


std::string makePrintable(const Word &word)
{
	std::string printable;
	size_t printable_size = word.size() > MaximumPrintableWordSize ? MaximumPrintableWordSize : word.size();
	std::copy(word.begin(), word.begin() + printable_size, std::back_inserter(printable));

	if( word.size() > printable.size() ){
		truncate(printable);
	} else if( printable.size() < MaximumPrintableWordSize ){
		fillWithTrailingWhitespace(printable, MaximumPrintableWordSize - printable.size());
	}

	return printable;
}

std::string makePrintable(const Occurrences &occurrences)
{
	std::string printable = "{";

	const std::vector<Sentence> &sentences = occurrences.get();
	printable += std::to_string(sentences.size());
	printable += ":";

	auto to_string = [](Sentence sentence){
		return std::to_string(sentence) + ",";
	};

	for( Sentence sentence : sentences ){
		printable += std::to_string(sentence) + ",";
	}

	printable.back() = '}';
	return printable;
}

std::string makePrintable(WordIndex index)
{
	char letter;
	size_t quantity;
	std::tie(letter, quantity) = convertWordIndexToEnglishLetters(index);

	std::string printable;

	if( quantity + 1 > MaximumPrintableIndexSize ){
		printable = "#######.";
	} else{
		printable = joinLettersOfConvertedIndex(letter, quantity);
		printable += ".";
	}

	if( printable.size() < MaximumPrintableIndexSize ){
		fillWithTrailingWhitespace(printable, MaximumPrintableIndexSize - printable.size());
	}

	return printable;
}
//END OF EXTERNAL FUNCTION DEFINITIONS