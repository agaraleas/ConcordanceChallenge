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

    std::optional<std::string> chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "Given");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "an");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "arbitrary");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "text");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "document");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "written");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "in");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "English,");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "write");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "a");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "program");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "that");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "will");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "generate");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "a");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "concordance,");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "i.e.");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "an");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "alphabetical");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "list");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "of");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "all");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "word");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "occurrences,");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "labeled");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "with");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "word");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "frequencies.");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "Bonus:");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "label");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "each");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "word");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "with");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "the");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "sentence");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "numbers");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "in");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "which");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "each");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "occurrence");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "appeared");

    chunk = traveller.getNext();
    EXPECT_FALSE(chunk);

    chunk = traveller.getNext();
    EXPECT_FALSE(chunk);
}

class DocumentTravellerSimpleSentence : public DocumentTravellerAbstractFixture
{
    std::string getText() const override
    {
        return "This is\na simple sentence.";
    }
};

TEST_F(DocumentTravellerSimpleSentence, TestPutBack)
{
    TextDocumentTraveller traveller(m_temp_file);

    std::optional<std::string> chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "This");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "is");

    traveller.putBack(*chunk);
    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "is");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "a");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "simple");

    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "sentence.");

    chunk = traveller.getNext();
    ASSERT_FALSE(chunk);

    traveller.putBack("sentence.");
    chunk = traveller.getNext();
    ASSERT_TRUE(chunk);
    EXPECT_EQ(*chunk, "sentence.");

    chunk = traveller.getNext();
    ASSERT_FALSE(chunk);
}

TEST(DocumentTravellerEdgeCases, NonExistingFile)
{
    TextDocumentTraveller traveller("/if/this/path/is/found/I/should/have/played/in/the/lottery/instead.txt");
    ASSERT_FALSE(traveller.getNext());
}
