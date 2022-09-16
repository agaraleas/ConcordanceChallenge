#include "Concordance.hpp"

#include <map>
#include <deque>
#include <algorithm>


//INTERNAL CLASS DECLARATIONS
//==========================================================================|
//							Concordance::Impl								|
//==========================================================================|
class Concordance::Impl
{
public:
	size_t size() const;
	bool equalsWith(const Concordance::Impl &other) const;
	void addOccurence(const Word &word, Sentence sentence);
	void forEachWord(const IteratorFunc &run_callback) const;

private:
	std::map<Word, Occurences> m_concordance;
};
//END OF INTERNAL CLASS DECLARATIONS`



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

void Concordance::Impl::addOccurence(const Word &word, Sentence sentence)
{
	Occurences &word_occurences = m_concordance[word];
	word_occurences << sentence;
}

void Concordance::Impl::forEachWord(const IteratorFunc &run_callback) const
{
	WordIndex index = 1;

	for( const auto &pair : m_concordance ){
		const Word &word = pair.first;
		const Occurences &occurences = pair.second;
		run_callback(index++, word, occurences);
	}
}
//END OF INTERNAL CLASS DEFINITIONS



//EXTERNAL CLASS DEFINITIONS
//==========================================================================|
//								Occurences									|
//==========================================================================|
Occurences &Occurences::operator<<(Sentence sentence)
{
	m_occurences.push_back(sentence);
	return *this;
}

bool Occurences::operator==(const Occurences &other) const
{
	return m_occurences == other.m_occurences;
}

const std::vector<Sentence> &Occurences::get() const
{
	return m_occurences;
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
	m_impl->addOccurence(word, sentence);
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
	Concordance concordance;
	//left intentionally as is for now in the concept of first creating a failing unit test (disabled)
	//and then implement
	return concordance;
}
//END OF EXTERNAL CLASS DEFINITIONS


