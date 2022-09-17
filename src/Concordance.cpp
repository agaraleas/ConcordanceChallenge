#include "Concordance.hpp"

#include <map>
#include <deque>
#include <algorithm>

#include "WordSanitizer.hpp"
#include "TextDocumentTraveller.hpp"


//INTERNAL CLASS DECLARATIONS
//==========================================================================|
//							Concordance::Impl								|
//==========================================================================|
class Concordance::Impl
{
public:
	size_t size() const;
	bool equalsWith(const Concordance::Impl &other) const;
	void addOccurrence(const Word &word, Sentence sentence);
	void forEachWord(const IteratorFunc &run_callback) const;

private:
	std::map<Word, Occurrences> m_concordance;
};
//END OF INTERNAL CLASS DECLARATIONS`


//INTERNAL AUXILIARY CLASSES AND FUNCTIONS
namespace
{

static bool isAbbreviation(const Word &word)
{
	return std::count(word.begin(), word.end(), '.') > 1;
}

static bool changesSentence(const DocumentElement &element)
{
	const Symbol *symbol = std::get_if<Symbol>(&element);
	return symbol ? changesSentence(*symbol) : false;
}

class ParsedElementVisitor
{
public:
	ParsedElementVisitor();

	void operator()(const Word &word);
	void operator()(const Symbol &symbol);

	const Concordance &getParsedConcordance() const;

private:
	Concordance m_concordance;
	DocumentElement m_previous_element;
	Sentence m_current_sentence = 1;
};

ParsedElementVisitor::ParsedElementVisitor() : m_concordance( Concordance::makeEmpty() )
{
}

void ParsedElementVisitor::operator()(const Word &word)
{
	m_concordance.add(word, m_current_sentence);
	m_previous_element = word;
}

void ParsedElementVisitor::operator()(const Symbol &symbol)
{
	if( changesSentence(symbol) && !changesSentence(m_previous_element) ){
		++m_current_sentence;
	}

	m_previous_element = symbol;
}

const Concordance &ParsedElementVisitor::getParsedConcordance() const
{
	return m_concordance;
}

}
//END OF INTERNAL AUXILIARY CLASSES AND FUNCTIONS


//INTERNAL CLASS DEFINITIONS
//==========================================================================|
//							Concordance::Impl								|
//==========================================================================|
size_t Concordance::Impl::size() const
{
	return m_concordance.size();
}

bool Concordance::Impl::equalsWith(const Concordance::Impl &other) const
{
	return m_concordance == other.m_concordance;
}

void Concordance::Impl::addOccurrence(const Word &word, Sentence sentence)
{
	Occurrences &word_occurrences = m_concordance[word];
	word_occurrences << sentence;
}

void Concordance::Impl::forEachWord(const IteratorFunc &run_callback) const
{
	WordIndex index = 1;

	for( const auto &pair : m_concordance ){
		const Word &word = pair.first;
		const Occurrences &occurrences = pair.second;
		run_callback(index++, word, occurrences);
	}
}
//END OF INTERNAL CLASS DEFINITIONS



//EXTERNAL CLASS DEFINITIONS
//==========================================================================|
//								Occurrences									|
//==========================================================================|
Occurrences &Occurrences::operator<<(Sentence sentence)
{
	m_occurrences.push_back(sentence);
	return *this;
}

bool Occurrences::operator==(const Occurrences &other) const
{
	return m_occurrences == other.m_occurrences;
}

const std::vector<Sentence> &Occurrences::get() const
{
	return m_occurrences;
}

//==========================================================================|
//								Concordance									|
//==========================================================================|
bool Concordance::operator==(const Concordance &other) const
{
	return m_impl->equalsWith(*other.m_impl);
}

bool Concordance::operator!=(const Concordance &other) const
{
	return !m_impl->equalsWith(*other.m_impl);
}

Concordance::Concordance()
{
	m_impl = std::make_unique<Impl>();
}

Concordance::~Concordance()
{
}

Concordance::Concordance(const Concordance &other)
{
	m_impl = std::make_unique<Impl>(*other.m_impl);
}

Concordance::Concordance(Concordance &&other) noexcept
{
	m_impl = std::move(other.m_impl);
}

Concordance &Concordance::operator=(const Concordance &other)
{
	if( this == &other ){
		return *this;
	}

	m_impl = std::make_unique<Impl>(*other.m_impl);
	return *this;
}

Concordance &Concordance::operator=(Concordance &&other) noexcept
{
	m_impl = std::move(other.m_impl);
	return *this;
}

size_t Concordance::size() const
{
	return m_impl->size();
}

void Concordance::forEachWord(const IteratorFunc &run_callback) const
{
	m_impl->forEachWord(run_callback);
}

void Concordance::add(const Word &word, const Sentence &sentence)
{
	WordSanitizer sanitizer;

	if( isAbbreviation(word) ){
		sanitizer.setAbbreviationSanitization();
	}

	Word sanitized = sanitizer.sanitize(word);
	m_impl->addOccurrence(sanitized, sentence);
}

Concordance Concordance::makeEmpty()
{
	return Concordance();
}

Concordance Concordance::makeFromSentences(const std::vector< std::vector<Word> > &sentences)
{
	Concordance concordance;
	
	size_t current_sentence = 1;

	for( const std::vector<Word> words_of_sentence : sentences ){
		for( const Word &word : words_of_sentence ){
			concordance.add(word, current_sentence);
		}

		++current_sentence;
	}

	return concordance;
}
Concordance Concordance::makeFromFile(const std::string &filepath)
{
	TextDocumentTraveller document_traveller(filepath);
	ParsedElementVisitor element_visitor;

	while( document_traveller.hasNext() ){
		DocumentElement element = document_traveller.getNext();
		std::visit(element_visitor, element);
	}
	return element_visitor.getParsedConcordance();
}
//END OF EXTERNAL CLASS DEFINITIONS


