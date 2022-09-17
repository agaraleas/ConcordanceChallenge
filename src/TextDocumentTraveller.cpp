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

	std::optional<std::string> getNext();
	void putBack(const std::string &taken);

private:
	void fillBuffer();

private:
	std::ifstream m_file_stream;
	std::deque<std::string> m_buffered_chunks;
};
//END OF INTERNAL CLASS DECLARATIONS`

//INTERNAL AUX FUNCTIONS
static bool isWhitespace(char c)
{
	return c == ' ' || c == '\n';
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

std::optional<std::string> TextDocumentTraveller::Impl::getNext()
{
	std::optional<std::string> next;

	if( m_buffered_chunks.empty() ){
		fillBuffer();
	}

	if( m_buffered_chunks.size() ){
		next = m_buffered_chunks.front();
		m_buffered_chunks.pop_front();
	}

	return next;
}

void TextDocumentTraveller::Impl::putBack(const std::string &taken)
{
	m_buffered_chunks.push_front(taken);
}

void TextDocumentTraveller::Impl::fillBuffer()
{
	if( !m_file_stream.is_open() ){
		return;
	}

	for( size_t i = 0; i < BufferedChunksSize; i++ ){
		std::string chunk = getNextChunk(m_file_stream);

		if( chunk.size() ){
			m_buffered_chunks.push_back(std::move(chunk));
		} else {
			break;
		}
	}
}
//END OF INTERNAL CLASS DEFINITIONS`


//EXTERNAL CLASS DEFINITIONS
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

std::optional<std::string> TextDocumentTraveller::getNext()
{
	return m_impl->getNext();
}

void TextDocumentTraveller::putBack(const std::string &taken)
{
	m_impl->putBack(taken);
}


//END OF EXTERNAL CLASS DEFINITIONS

