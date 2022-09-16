#ifndef CONCORDANCE_HPP
#define CONCORDANCE_HPP

//Include Headers
#include <string>
#include <vector>
#include <memory>
#include <functional>

//Typedefs
using Sentence = size_t;
using Word = std::string;
using WordIndex = size_t;

//==========================================================================|
//								Occurences									|
//==========================================================================|
// @brief: Object which holds the sentences of a found word					|
//==========================================================================|

class Occurences
{
public:
	const std::vector<Sentence> &get() const;
	Occurences &operator << (Sentence sentence);
	bool operator == (const Occurences &other) const;

private:
	std::vector<Sentence> m_occurences;
};

//==========================================================================|
//								Concordance									|
//==========================================================================|
// @brief: Object which depicts a collection of words found in a text		|
//		   document along with their occurences inside that document		|
//==========================================================================|

class Concordance
{
public:
	static Concordance makeEmpty();
	static Concordance makeFromSentences(const std::vector< std::vector<Word> > &sentences);
	static Concordance makeFromFile(const std::string &filepaths);

	bool operator == (const Concordance &other) const;
	bool operator != (const Concordance &other) const;

	~Concordance();
	Concordance(const Concordance &other);
	Concordance(Concordance &&other) noexcept;
	Concordance &operator=(const Concordance &other);
	Concordance &operator=(Concordance &&other) noexcept;

	size_t size() const;

	using IteratorFunc = std::function<void(WordIndex, const Word &, const Occurences &)>;
	void forEachWord(const IteratorFunc &run_callback) const;

	void add(const Word&, const Sentence&);

private:
	Concordance();

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
};


#endif