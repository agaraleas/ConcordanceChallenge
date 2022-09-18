#include "TextDocumentTraveller.hpp"
#include <deque>
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

//INTERNAL AUX FUNCTIONS
namespace
{

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

static bool isDot(const std::string::const_iterator &current, 
				  const std::string &chunk)
{
	if( current == chunk.end() ) return false;
	return *current == '.';
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

static std::string::const_iterator findEndOfAbbreviation(std::string::const_iterator current,
														 const std::string &chunk)
{
	auto should_terminate_abbreviation = [](char c){
		return isWordTerminatingCharacter(c) && (c != '.');
	};

	return std::find_if(current, chunk.end(), should_terminate_abbreviation);
}

static std::string::const_iterator findEndOfElement(std::string::const_iterator current,
													const std::string &chunk)
{
	if( isSymbol(*current) ){
		return ++current;
	
	} else {
		auto word_end = std::find_if(current, chunk.end(), isWordTerminatingCharacter);
		
		if( isDot(word_end, chunk) && nextLetterIsLowercase(word_end, chunk) ){
			word_end = findEndOfAbbreviation(word_end, chunk);
		}
		
		return word_end;
	}
}

static DocumentElement evaluate(const std::string::const_iterator &begin,
								const std::string::const_iterator &end)
{
	if( std::distance(begin, end) == 1 && isSymbol(*begin) ){
		return Symbol(*begin);
	} else {
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

}
//END OF INTERNAL AUX FUNCTIONS

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


