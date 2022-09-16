#ifndef OUTPUTFORMATTINGS_HPP
#define OUTPUTFORMATTINGS_HPP

//Include Headers
#include <string>

//Typedefs
using Sentence = size_t;
using Word = std::string;
using WordIndex = size_t;

//Forward Declaratinos
class Occurences;

//==========================================================================|
//						    Formatting Functions							|
//==========================================================================|
std::string makePrintable(WordIndex index);
std::string makePrintable(const Word &word);
std::string makePrintable(const Occurences &occurences);

#endif