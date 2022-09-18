#include "TextDocumentTraveller.hpp"
#include <deque>
#include <vector>
#include <fstream>

static const size_t BufferedChunksSize = 20;

//INTERNAL CLASS DECLARATIONS
//==========================================================================|
//						DocumentTraveller::Impl								|
//==========================================================================|
class TextDocumentTraveller::Impl
{
public:
	Impl(){}
	~Impl();

	void openFile(const std::string &filepath);

	bool hasNext();
	DocumentElement getNext();

private:
	void fillBuffer();

private:
	std::ifstream m_file_stream;
	std::deque<DocumentElement> m_parse_buffer;
};
//END OF INTERNAL CLASS DECLARATIONS`


//INTERNAL AUX CLASSES AND FUNCTIONS
namespace
{

//INTERNAL AUX CLASS DECLARATIONS
//==========================================================================|
//								EndCalculator								|
//==========================================================================|
// @brief: Calculates the end of a DocumentElement of a chunk.				|
//		   A document chunk can contain more than one DocumentElements,like:|
//		   'well,no' --> 3 document elements: 'well' - ',' - 'no'			|
//		   The EndCalculator returns the end iterator of the DocumentElement|
//		   under consideration												|
//==========================================================================|

class ElementEndCalculator
{
public:
	ElementEndCalculator(const std::string::const_iterator &start, const std::string &document_chunk);
	virtual ~ElementEndCalculator() {}

	std::string::const_iterator calcEndOfDocumentElement();

protected:
	const std::string::const_iterator &getStart() const;
	const std::string &getDocumentChunk() const;
	
	enum class CharacterHandling
	{
		Consume,
		MarkAsEnd,
	};
	virtual CharacterHandling checkCharacter(const std::string::const_iterator &current) = 0;

private:
	std::string::const_iterator m_element_start;
	const std::string &m_document_chunk;
};

//==========================================================================|
//							SymbolEndCalculator								|
//==========================================================================|
// @brief: SymbolEndCalculator - Concrete class which calculates the end of |
//		   a Symbol.														|
//		   A symbol always consists of a single character					|
//==========================================================================|

class SymbolEndCalculator : public ElementEndCalculator
{
public:
	SymbolEndCalculator(const std::string::const_iterator &start, const std::string &document_chunk);
private:
	CharacterHandling checkCharacter(const std::string::const_iterator &current);
};


//==========================================================================|
//							WordEndCalculator								|
//==========================================================================|
// @brief: WordEndCalculator - Concrete class which calculates the end      |
//		   of a word. A word does not necessarily ends on the end of the 	|
//		   chunk, or on  a word terminating character.						|
//		   Special handling needs to be done for abbreviations or words with| 
//		   special characters												|
//==========================================================================|

class WordEndCalculator : public ElementEndCalculator
{
public:
	WordEndCalculator(const std::string::const_iterator &start, const std::string &document_chunk);

private:
	CharacterHandling checkCharacter(const std::string::const_iterator &current);

	enum class WordType
	{
		EnglishWord,
		Abbreviation,
		SpecialCharacters,
	};
	
	void updateWordType(const std::string::const_iterator &current);
	
private:
	WordType m_type = WordType::EnglishWord;
};
//END OF INTERNAL AUX CLASS DECLARATIONS



//INTERNAL AUX FUNCTIONS
std::deque<DocumentElement> &operator<<(std::deque<DocumentElement> &collection, DocumentElement &&element)
{
	collection.push_back( std::move(element) );
	return collection;
}

std::deque<DocumentElement> &operator<<(std::deque<DocumentElement> &collection, std::deque<DocumentElement> &&other)
{
	std::move(other.begin(), other.end(), std::back_inserter(collection));
	return collection;
}

static bool isWhitespace(char c)
{
	return c == ' ' || c == '\n' || c == '\t';
}

static bool isSymbol(char c)
{
	return !std::isalnum(static_cast<unsigned char>(c));
}

static bool isDot(char c)
{
	return c == '.';
}

static bool isWordTerminatingCharacter(char c)
{
	return c == '.'  ||
		   c == '!'  ||
		   c == '?'  ||
		   c == ','  ||
		   c == '('  ||
		   c == ')'  ||
		   c == '['  ||
		   c == ']'  ||
		   c == '{'  ||
		   c == '}'  ||
		   c == ';'  ||
		   c == ':'  ||
		   c == '"'  ||
		   c == '<'  ||
		   c == '>';
}

static bool canTerminateAbbreviation(const std::string::const_iterator &current)
{
	if( isDot(*current) ){
		std::string::const_iterator previous = current - 1;
		return isDot(*previous);

	} else {
		return isWordTerminatingCharacter(*current);
	}
}

static std::string getNextChunk(std::ifstream &stream)
{
	std::string chunk;

	char c;
	while( stream.get(c) ){
		if( isWhitespace(c) ){
			if( chunk.size() ){
				break;
			}

		} else{
			chunk.push_back(c);
		}
	}

	return chunk;
}

static bool nextLetterIsLowercase(std::string::const_iterator current, const std::string &chunk)
{
	std::string::const_iterator next = current != chunk.end() ? current + 1 : chunk.end();

	if( next != chunk.end() && std::isalpha(static_cast<unsigned char>(*next)) ){
		return std::islower(*next);
	}

	return false;
}

namespace EndCalculatorFactory
{

	std::unique_ptr<ElementEndCalculator> getCalculator(std::string::const_iterator start,
		const std::string &document_chunk)
	{
		std::unique_ptr<ElementEndCalculator> calculator;

		if( isSymbol(*start) ){
			calculator = std::make_unique<SymbolEndCalculator>(start, document_chunk);
		} else{
			calculator = std::make_unique<WordEndCalculator>(start, document_chunk);
		}

		return calculator;
	}

}

static std::string::const_iterator findEndOfElement(std::string::const_iterator current,
	const std::string &chunk)
{
	std::unique_ptr<ElementEndCalculator> calculator = EndCalculatorFactory::getCalculator(current, chunk);
	return calculator->calcEndOfDocumentElement();
}

static DocumentElement evaluate(const std::string::const_iterator &begin,
	const std::string::const_iterator &end)
{
	if( std::distance(begin, end) == 1 && isSymbol(*begin) ){
		return Symbol(*begin);
	} else{
		return Word(begin, end);
	}
}

static std::deque<DocumentElement> splitChunkIntoDocumentElements(const std::string &chunk)
{
	std::deque<DocumentElement> parsed_elements;

	std::string::const_iterator start = chunk.begin();
	std::string::const_iterator end = findEndOfElement(start, chunk);

	while( end != chunk.end() ){
		parsed_elements << evaluate(start, end);
		start = end;
		end = findEndOfElement(start, chunk);
	}

	if( start != chunk.end() ){
		parsed_elements << evaluate(start, end);
	}

	return parsed_elements;
}
//END OF INTERNAL AUX FUNCTIONS


//INTERNAL AUX CLASS DEFINITIONS
ElementEndCalculator::ElementEndCalculator(const std::string::const_iterator &start,
							 const std::string &document_chunk) : m_element_start(start), 
																  m_document_chunk(document_chunk)
{
}

SymbolEndCalculator::SymbolEndCalculator(const std::string::const_iterator &start,
										 const std::string &document_chunk) : ElementEndCalculator(start, document_chunk)
{
}

ElementEndCalculator::CharacterHandling SymbolEndCalculator::checkCharacter(const std::string::const_iterator &current)
{
	return current == getStart() + 1 ? CharacterHandling::MarkAsEnd : CharacterHandling::Consume;
}

WordEndCalculator::WordEndCalculator(const std::string::const_iterator &start,
									 const std::string &document_chunk) : ElementEndCalculator(start, document_chunk)
{
}

ElementEndCalculator::CharacterHandling WordEndCalculator::checkCharacter(const std::string::const_iterator &current)
{
	updateWordType(current);

	switch( m_type ){
	case WordType::EnglishWord:
		return isWordTerminatingCharacter(*current) ? CharacterHandling::MarkAsEnd : CharacterHandling::Consume;
	case WordType::Abbreviation:
		return canTerminateAbbreviation(current) ? CharacterHandling::MarkAsEnd : CharacterHandling::Consume;

	default:
		return CharacterHandling::Consume;
	}
}

void WordEndCalculator::updateWordType(const std::string::const_iterator &current)
{
	if( isSymbol(*current) && !isWordTerminatingCharacter(*current) ){
		m_type = WordType::SpecialCharacters;
	} else if( isDot(*current) && nextLetterIsLowercase(current, getDocumentChunk()) ){
		m_type = WordType::Abbreviation;
	}
}

std::string::const_iterator ElementEndCalculator::calcEndOfDocumentElement()
{
	auto current = getStart();
	std::string::const_iterator end = getDocumentChunk().end();

	while( current != end && checkCharacter(current) == CharacterHandling::Consume ){
		++current;
	}

	return current;
}

const std::string::const_iterator &ElementEndCalculator::getStart() const
{
	return m_element_start;
}
const std::string &ElementEndCalculator::getDocumentChunk() const
{
	return m_document_chunk;
}
//END OFINTERNAL AUX CLASS DEFINITIONS

} //END OF ANONYMOUS NAMESPACE
//END OF INTERNAL AUX CLASSES AND FUNCTIONS


//INTERNAL CLASS DEFINITIONS
TextDocumentTraveller::Impl::~Impl()
{
	if( m_file_stream.is_open() ){
		m_file_stream.close();
	}
}

void TextDocumentTraveller::Impl::openFile(const std::string &filepath)
{
	m_file_stream.open(filepath);
}

bool TextDocumentTraveller::Impl::hasNext()
{
	if( m_parse_buffer.empty() ){
		fillBuffer();
	}

	return m_parse_buffer.size();
}

DocumentElement TextDocumentTraveller::Impl::getNext()
{
	DocumentElement next;

	if( m_parse_buffer.empty() ){
		fillBuffer();
	}

	if( m_parse_buffer.size() ){
		next = m_parse_buffer.front();
		m_parse_buffer.pop_front();
	}

	return next;
}

void TextDocumentTraveller::Impl::fillBuffer()
{
	if( !m_file_stream.is_open() ){
		return;
	}

	for( size_t i = 0; i < BufferedChunksSize; i++ ){
		std::string chunk = getNextChunk(m_file_stream);

		if( chunk.size() ){
			m_parse_buffer << splitChunkIntoDocumentElements(chunk);
		} else {
			break;
		}
	}
}
//END OF INTERNAL CLASS DEFINITIONS`


//EXTERNAL CLASS DEFINITIONS
//==========================================================================|
//								  Symbol									|
//==========================================================================|
Symbol::Symbol(char c)
{
	m_value = c;
}

char Symbol::get() const
{
	return m_value;
}

//==========================================================================|
//							DocumentTraveller								|
//==========================================================================|
TextDocumentTraveller::TextDocumentTraveller(const std::string &filepath)
{
	m_impl = std::make_unique<Impl>();
	m_impl->openFile(filepath);
}

TextDocumentTraveller::~TextDocumentTraveller()
{
}

bool TextDocumentTraveller::hasNext()
{
	return m_impl->hasNext();
}

DocumentElement TextDocumentTraveller::getNext()
{
	return m_impl->getNext();
}
//END OF EXTERNAL CLASS DEFINITIONS

//EXTERNAL FUNCTION DEFINITIONS
bool changesSentence(const Symbol &symbol)
{
	return symbol.get() == '.' ||
		   symbol.get() == '!' ||
		   symbol.get() == '?' ||
		   symbol.get() == ';';
}
//END OF EXTERNAL FUNCTION DEFINITIONS


