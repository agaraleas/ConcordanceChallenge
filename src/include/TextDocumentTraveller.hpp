#ifndef TEXTDOCUMENTTRAVELLER_HPP
#define TEXTDOCUMENTTRAVELLER_HPP

//Include Headers
#include <memory>
#include <string>
#include <optional>

//==========================================================================|
//						   TextDocumentTraveller							|
//==========================================================================|
// @brief: Opens a plain text document and travels it in chunks. A chunk is	|
//		   considered any series of alphanumeric characters till they reach |
//		   whitespace														|
//==========================================================================|
class TextDocumentTraveller
{
public:
	TextDocumentTraveller(const std::string &filepath);
	~TextDocumentTraveller();
	TextDocumentTraveller(const TextDocumentTraveller &other) = delete;
	TextDocumentTraveller &operator=(const TextDocumentTraveller &other) = delete;

	std::optional<std::string> getNext();
	void putBack(const std::string &taken);

private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
};

#endif