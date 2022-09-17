#include <gtest/gtest.h>
#include <fstream>
#include "TextDocumentTraveller.hpp"

class DocumentTravellerAbstractFixture : public ::testing::Test {
public:
    DocumentTravellerAbstractFixture() {}
    virtual ~DocumentTravellerAbstractFixture() {}
    virtual std::string getText() const = 0;

    void SetUp() {
        m_temp_file = std::tmpnam(nullptr);

        std::ofstream out(m_temp_file);
        out << getText();
        out.close();
    }

    void TearDown() {
        remove(m_temp_file.c_str());
    }

    std::string m_temp_file;
};

class DocumentTravellerExample : public DocumentTravellerAbstractFixture
{
    std::string getText() const override
    {
        std::string dataset = "Given an arbitrary text document written in English, write a program that will generate a \n";
        dataset += "concordance, i.e. an alphabetical list of all word occurrences, labeled with word \n";
        dataset += "frequencies. \n\n";
        dataset += "Bonus: label each word with the sentence numbers in which each occurrence appeared";
        return dataset;
    }
};

TEST_F(DocumentTravellerExample, TravelDocumentOfExample)
{
    TextDocumentTraveller traveller(m_temp_file);

    DocumentElement parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "Given");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "an");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "arbitrary");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "text");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "document");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "written");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "in");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "English");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Symbol>(&parsed));
    EXPECT_EQ(std::get<Symbol>(parsed).get(), ',');

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "write");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "a");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "program");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "that");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "will");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "generate");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "a");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "concordance");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Symbol>(&parsed));
    EXPECT_EQ(std::get<Symbol>(parsed).get(), ',');

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "i.e.");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "an");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "alphabetical");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "list");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "of");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "all");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "word");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "occurrences");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Symbol>(&parsed));
    EXPECT_EQ(std::get<Symbol>(parsed).get(), ',');

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "labeled");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "with");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "word");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "frequencies");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Symbol>(&parsed));
    EXPECT_EQ(std::get<Symbol>(parsed).get(), '.');

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "Bonus");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Symbol>(&parsed));
    EXPECT_EQ(std::get<Symbol>(parsed).get(), ':');

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "label");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "each");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "word");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "with");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "the");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "sentence");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "numbers");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "in");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "which");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "each");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "occurrence");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "appeared");

    EXPECT_FALSE(traveller.hasNext());
}

class DocumentTravellerSimpleSentence : public DocumentTravellerAbstractFixture
{
    std::string getText() const override
    {
        return "This is\na simple sentence.";
    }
};

TEST_F(DocumentTravellerSimpleSentence, TestSimpleSentence)
{
    TextDocumentTraveller traveller(m_temp_file);

    DocumentElement parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "This");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "is");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "a");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "simple");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Word>(&parsed));
    EXPECT_EQ(std::get<Word>(parsed), "sentence");

    parsed = traveller.getNext();
    ASSERT_TRUE(std::get_if<Symbol>(&parsed));
    EXPECT_EQ(std::get<Symbol>(parsed).get(), '.');

    EXPECT_FALSE(traveller.hasNext());
}

TEST(DocumentTravellerEdgeCases, NonExistingFile)
{
    TextDocumentTraveller traveller("/if/this/path/is/found/I/should/have/played/in/the/lottery/instead.txt");
    ASSERT_FALSE(traveller.hasNext());
}

TEST(SymbolStruct, BasicOperations)
{
    Symbol c;
    EXPECT_EQ(c.get(), 0);

    Symbol n('\n');
    EXPECT_EQ(n.get(), '\n');
}

TEST(SymbolStruct, SentenceChange)
{
    EXPECT_TRUE(changesSentence(Symbol('!').get()));
    EXPECT_FALSE(changesSentence(Symbol('"').get()));
    EXPECT_FALSE(changesSentence(Symbol('#').get()));
    EXPECT_FALSE(changesSentence(Symbol('$').get()));
    EXPECT_FALSE(changesSentence(Symbol('%').get()));
    EXPECT_FALSE(changesSentence(Symbol('&').get()));
    EXPECT_FALSE(changesSentence(Symbol('\'').get()));
    EXPECT_FALSE(changesSentence(Symbol('(').get()));
    EXPECT_FALSE(changesSentence(Symbol(')').get()));
    EXPECT_FALSE(changesSentence(Symbol('*').get()));
    EXPECT_FALSE(changesSentence(Symbol('+').get()));
    EXPECT_FALSE(changesSentence(Symbol(',').get()));
    EXPECT_FALSE(changesSentence(Symbol('-').get()));
    EXPECT_TRUE(changesSentence(Symbol('.').get()));
    EXPECT_FALSE(changesSentence(Symbol('/').get()));
    EXPECT_FALSE(changesSentence(Symbol('0').get()));
    EXPECT_FALSE(changesSentence(Symbol('1').get()));
    EXPECT_FALSE(changesSentence(Symbol('2').get()));
    EXPECT_FALSE(changesSentence(Symbol('3').get()));
    EXPECT_FALSE(changesSentence(Symbol('4').get()));
    EXPECT_FALSE(changesSentence(Symbol('5').get()));
    EXPECT_FALSE(changesSentence(Symbol('6').get()));
    EXPECT_FALSE(changesSentence(Symbol('7').get()));
    EXPECT_FALSE(changesSentence(Symbol('8').get()));
    EXPECT_FALSE(changesSentence(Symbol('9').get()));
    EXPECT_FALSE(changesSentence(Symbol(':').get()));
    EXPECT_TRUE(changesSentence(Symbol(';').get()));
    EXPECT_FALSE(changesSentence(Symbol('<').get()));
    EXPECT_FALSE(changesSentence(Symbol('=').get()));
    EXPECT_FALSE(changesSentence(Symbol('>').get()));
    EXPECT_TRUE(changesSentence(Symbol('?').get()));
    EXPECT_FALSE(changesSentence(Symbol('@').get()));
    EXPECT_FALSE(changesSentence(Symbol('A').get()));
    EXPECT_FALSE(changesSentence(Symbol('B').get()));
    EXPECT_FALSE(changesSentence(Symbol('C').get()));
    EXPECT_FALSE(changesSentence(Symbol('D').get()));
    EXPECT_FALSE(changesSentence(Symbol('E').get()));
    EXPECT_FALSE(changesSentence(Symbol('F').get()));
    EXPECT_FALSE(changesSentence(Symbol('G').get()));
    EXPECT_FALSE(changesSentence(Symbol('H').get()));
    EXPECT_FALSE(changesSentence(Symbol('I').get()));
    EXPECT_FALSE(changesSentence(Symbol('J').get()));
    EXPECT_FALSE(changesSentence(Symbol('K').get()));
    EXPECT_FALSE(changesSentence(Symbol('L').get()));
    EXPECT_FALSE(changesSentence(Symbol('M').get()));
    EXPECT_FALSE(changesSentence(Symbol('N').get()));
    EXPECT_FALSE(changesSentence(Symbol('O').get()));
    EXPECT_FALSE(changesSentence(Symbol('P').get()));
    EXPECT_FALSE(changesSentence(Symbol('Q').get()));
    EXPECT_FALSE(changesSentence(Symbol('R').get()));
    EXPECT_FALSE(changesSentence(Symbol('S').get()));
    EXPECT_FALSE(changesSentence(Symbol('T').get()));
    EXPECT_FALSE(changesSentence(Symbol('U').get()));
    EXPECT_FALSE(changesSentence(Symbol('V').get()));
    EXPECT_FALSE(changesSentence(Symbol('W').get()));
    EXPECT_FALSE(changesSentence(Symbol('X').get()));
    EXPECT_FALSE(changesSentence(Symbol('Y').get()));
    EXPECT_FALSE(changesSentence(Symbol('Z').get()));
    EXPECT_FALSE(changesSentence(Symbol('[').get()));
    EXPECT_FALSE(changesSentence(Symbol('\\').get()));
    EXPECT_FALSE(changesSentence(Symbol(']').get()));
    EXPECT_FALSE(changesSentence(Symbol('^').get()));
    EXPECT_FALSE(changesSentence(Symbol('_').get()));
    EXPECT_FALSE(changesSentence(Symbol('`').get()));
    EXPECT_FALSE(changesSentence(Symbol('a').get()));
    EXPECT_FALSE(changesSentence(Symbol('b').get()));
    EXPECT_FALSE(changesSentence(Symbol('c').get()));
    EXPECT_FALSE(changesSentence(Symbol('d').get()));
    EXPECT_FALSE(changesSentence(Symbol('e').get()));
    EXPECT_FALSE(changesSentence(Symbol('f').get()));
    EXPECT_FALSE(changesSentence(Symbol('g').get()));
    EXPECT_FALSE(changesSentence(Symbol('h').get()));
    EXPECT_FALSE(changesSentence(Symbol('i').get()));
    EXPECT_FALSE(changesSentence(Symbol('j').get()));
    EXPECT_FALSE(changesSentence(Symbol('k').get()));
    EXPECT_FALSE(changesSentence(Symbol('l').get()));
    EXPECT_FALSE(changesSentence(Symbol('m').get()));
    EXPECT_FALSE(changesSentence(Symbol('n').get()));
    EXPECT_FALSE(changesSentence(Symbol('o').get()));
    EXPECT_FALSE(changesSentence(Symbol('p').get()));
    EXPECT_FALSE(changesSentence(Symbol('q').get()));
    EXPECT_FALSE(changesSentence(Symbol('r').get()));
    EXPECT_FALSE(changesSentence(Symbol('s').get()));
    EXPECT_FALSE(changesSentence(Symbol('t').get()));
    EXPECT_FALSE(changesSentence(Symbol('u').get()));
    EXPECT_FALSE(changesSentence(Symbol('v').get()));
    EXPECT_FALSE(changesSentence(Symbol('w').get()));
    EXPECT_FALSE(changesSentence(Symbol('x').get()));
    EXPECT_FALSE(changesSentence(Symbol('y').get()));
    EXPECT_FALSE(changesSentence(Symbol('z').get()));
    EXPECT_FALSE(changesSentence(Symbol('{').get()));
    EXPECT_FALSE(changesSentence(Symbol('|').get()));
    EXPECT_FALSE(changesSentence(Symbol('}').get()));
    EXPECT_FALSE(changesSentence(Symbol('~').get()));
}