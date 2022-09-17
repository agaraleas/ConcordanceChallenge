#ifndef TEXTDOCUMENTTRAVELLER_HPP
#define TEXTDOCUMENTTRAVELLER_HPP

//Include Headers
#include <memory>
#include <string>
#include <variant>

//==========================================================================|
//									Symbol									|
//==========================================================================|
// @brief: Object which represents a non alphanumeric character				|
//		   Intention is to be able to seperate Symbols from Words in a 		|
//		   variant. This is why an explicit type is needed					|
//==========================================================================|

class Symbol
{
public:
	Symbol(){}
	Symbol(char c);
	char get() const;

private:
	char m_value = 0;
};

bool changesSentence(const Symbol &symbol);


//==========================================================================|
//									ParsedElement							|
//==========================================================================|
// @brief: A variant which can holds either words of a document or symbols	|
//==========================================================================|
using Word = std::string;
using DocumentElement = std::variant<Word, Symbol>;


//==========================================================================|
//						   TextDocumentTraveller							|
//==========================================================================|
// @brief: Opens a plain text document and travels it in elements. An		|
//		   element is considered an english word or a symbol. Numbers are	|
//		   treated as words.												|
//==========================================================================|
class TextDocumentTraveller
{
public:
	TextDocumentTraveller(const std::string &filepath);
	~TextDocumentTraveller();
	TextDocumentTraveller(const TextDocumentTraveller &other) = delete;
	TextDocumentTraveller &operator=(const TextDocumentTraveller &other) = delete;

	bool hasNext();
	DocumentElement getNext();

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
};

#endif