#include <gtest/gtest.h>
#include <fstream>
#include "Concordance.hpp"

TEST(ConcordanceTests, OccurrenceInsertions)
{
    Occurrences occurrences;
    occurrences << 1 << 3;

    const std::vector<Sentence> &sentences = occurrences.get();
    EXPECT_EQ(sentences.size(), 2);
    EXPECT_EQ(sentences.front(), 1);
    EXPECT_EQ(sentences.back(), 3);
}

TEST(ConcordanceTests, OccurrenceEquality)
{
    Occurrences occurrence1;
    occurrence1 << 1 << 3;

    Occurrences occurrence2;
    occurrence2 << 1 << 3;

    EXPECT_EQ(occurrence1 == occurrence2, true);

    occurrence2 << 4 << 5;

    EXPECT_EQ(occurrence1 == occurrence2, false);
}

static Concordance generateConcordanceForDatasetA()
{
    std::vector < std::vector<Word> > generator = {
        {"This", "is", "a", "simple", "dataset"},
        {"It", "only", "contains", "3", "sentences"},
        {"This", "is", "the", "last"}
    };

    return Concordance::makeFromSentences(generator);
}

static std::map<Word, std::vector<Sentence> > getExpectationOfDatasetA()
{
    std::map<Word, std::vector<Sentence> > expectation = {
        {"a", {1}},
        {"contains", {2}},
        {"dataset", {1}},
        {"it", {2}},
        {"is", {1,3}},
        {"last", {3}},
        {"only", {2}},
        {"sentences", {2}},
        {"simple", {1}},
        {"the", {3}},
        {"this", {1,3}}
    };

    return expectation;
}

#define COMPARE_CONCORDANCE_WITH_EXPECTATION(CONCORDANCE, EXPECTATION)                              \
EXPECT_EQ(CONCORDANCE.size(), EXPECTATION.size());                                                  \
auto current = EXPECTATION.begin();                                                                 \
                                                                                                    \
CONCORDANCE.forEachWord([&](WordIndex index, const Word &word, const Occurrences &occurrences){       \
    ASSERT_LE(index, EXPECTATION.size());                                                           \
    ASSERT_TRUE(CONCORDANCE.exists(current->first)) << "Missing: "  << current->first;              \
    ASSERT_TRUE(EXPECTATION.count(word)) << "Excessive word: "  << word;              \
    EXPECT_EQ(word, current->first);                                                                \
    EXPECT_EQ(occurrences.get(), current->second) << "Word: " << word;                                         \
    EXPECT_EQ(index, std::distance(EXPECTATION.begin(), current) + 1);                              \
    ++current;                                                                                       \
})                                                                                                  \

TEST(ConcordanceTests, ConcordanceCreationFromSentences_A)
{
    Concordance concordance = generateConcordanceForDatasetA();
    std::map<Word, std::vector<Sentence> > expectation = getExpectationOfDatasetA();
    COMPARE_CONCORDANCE_WITH_EXPECTATION(concordance, expectation);
}

static Concordance generateConcordanceForDatasetB()
{
    std::vector < std::vector<Word> > generator = {
        {"This", "is", "more", "complex"},
        {"Will", "it", "succeed"},
        {"I", "hope", "so"},
        {"It", "can", "get", "difficult"}
    };

    return Concordance::makeFromSentences(generator);
}

static std::map<Word, std::vector<Sentence> > getExpectationOfDatasetB()
{
    std::map<Word, std::vector<Sentence> > expectation = {
        {"can", {4}},
        {"complex", {1}},
        {"difficult", {4}},
        {"get", {4}},
        {"hope", {3}},
        {"i", {3}},
        {"is", {1}},
        {"it", {2,4}},
        {"more", {1}},
        {"so", {3}},
        {"succeed", {2}},
        {"this", {1}},
        {"will", {2}},
    };

    return expectation;
}

TEST(ConcordanceTests, ConcordanceCreationFromSentences_B)
{
    Concordance concordance = generateConcordanceForDatasetB();
    std::map<Word, std::vector<Sentence> > expectation = getExpectationOfDatasetB();
    COMPARE_CONCORDANCE_WITH_EXPECTATION(concordance, expectation);
}

class TestFileWritterFixture : public ::testing::Test
{
public:
    TestFileWritterFixture() {}
    virtual ~TestFileWritterFixture() {}
    virtual std::string getDataset() const = 0;

    void SetUp() 
    {
        m_temp_file = std::tmpnam(nullptr);

        std::ofstream out(m_temp_file);
        out << getDataset();
        out.close();
    }

    void TearDown() 
    {
        remove(m_temp_file.c_str());
    }

    std::string m_temp_file;
};

class GivenExampleFixture : public TestFileWritterFixture 
{
public:
    std::string getDataset() const override
    {
        std::string dataset = "Given an arbitrary text document written in English, write a program that will generate a \n";
        dataset += "concordance, i.e. an alphabetical list of all word occurrences, labeled with word \n";
        dataset += "frequencies. \n\n";
        dataset += "Bonus: label each word with the sentence numbers in which each occurrence appeared";
        return dataset;
    }
};

static std::map<Word, std::vector<Sentence> > getExpectationOfExampleDataset()
{
    std::map<Word, std::vector<Sentence> > expectation = {
        {"a", {1,1}},
        {"all", {1}},
        {"alphabetical", {1}},
        {"an", {1,1}},
        {"appeared", {2}},
        {"arbitrary", {1}},
        {"bonus", {2}},
        {"concordance", {1}},
        {"document", {1}},
        {"each", {2,2}},
        {"english", {1}},
        {"frequencies", {1}},
        {"generate", {1}},
        {"given", {1}},
        {"i.e.", {1}},
        {"in", {1,2}},
        {"label", {2}},
        {"labeled", {1}},
        {"list", {1}},
        {"numbers", {2}},
        {"occurrence", {2}},
        {"occurrences", {1}},
        {"of", {1}},
        {"program", {1}},
        {"sentence", {2}},
        {"text", {1}},
        {"that", {1}},
        {"the", {2}},
        {"which", {2}},
        {"will", {1}},
        {"with", {1,2}},
        {"word", {1,1,2}},
        {"write", {1}},
        {"written", {1}},
    };

    return expectation;
}

TEST_F(GivenExampleFixture, ExampleDataset)
{
    Concordance concordance = Concordance::makeFromFile(m_temp_file);
    std::map<Word, std::vector<Sentence> > expectation = getExpectationOfExampleDataset();
    COMPARE_CONCORDANCE_WITH_EXPECTATION(concordance, expectation);
}

class EllipsisHandlingOfSentenceFixture : public TestFileWritterFixture
{
public:
    std::string getDataset() const override
    {
        return "It is now .... too late";
    }
};

static std::map<Word, std::vector<Sentence> > getExpectationOfEllipsisHandlingOfSentence()
{
    std::map<Word, std::vector<Sentence> > expectation = {
        {"it", {1}},
        {"is", {1}},
        {"late", {1}},
        {"now", {1}},
        {"too", {1}},
    };

    return expectation;
}

TEST_F(EllipsisHandlingOfSentenceFixture, EllipsisShouldNotChangeSentence)
{
    Concordance concordance = Concordance::makeFromFile(m_temp_file);
    std::map<Word, std::vector<Sentence> > expectation = getExpectationOfEllipsisHandlingOfSentence();
    COMPARE_CONCORDANCE_WITH_EXPECTATION(concordance, expectation);
}

class ExtremeDatasetFixture : public TestFileWritterFixture
{
public:
    std::string getDataset() const override
    {
        /*1*/std::string dataset = "It is now ... too late.";
        /*2*/dataset += "This idiot forgot a space(!!!) on start of\nthe sentence!\n";
        /*3*/dataset += "I should be on sentence 3, right? ";
        /*4*/dataset += "Or am I saying b@11$h!t ? \n";
        /*5*/dataset += "Here is a list:\n\ta. ";
        /*6*/dataset += "I should do this.\n\tb. ";
        /*7*/dataset += "I should call at 067894254.\n";
        /*8*/dataset += "This program is vulnerable to hyphenation. "; //Revert to hyphe-\nnation when program supports it
        /*9*/dataset += "But it is known for now...\n";
        /*10*/dataset += "Here is the last symbol that should change sentence;";
        /*11*/dataset += "A new one should start now.\n";
        /*12*/dataset += "Angelo's code is probably not handling this correctly yet.\n";
        /*13*/dataset += "Env paths like %USER%/mypath.txt should be discarded.\n";
        return dataset;
    }
};

static std::map<Word, std::vector<Sentence> > getExpectationOfExtremeDataset()
{
    std::map<Word, std::vector<Sentence> > expectation = {
        {"a", {2,5,5,11}},
        {"am", {4}},
        {"angelo's", {12}},
        {"at", {7}},
        {"b", {6}},
        {"be", {3, 13}},
        {"but", {9}},
        {"call", {7}},
        {"change", {10}},
        {"code", {12}},
        {"correctly", {12}},
        {"do", {6}},
        {"discarded", {13}},
        {"env", {13}},
        {"be", {3}},
        {"for", {9}},
        {"forgot", {2}},
        {"handling", {12}},
        {"here", {5,10}},
        {"hyphenation", {8}},
        {"idiot", {2}},
        {"i", {3,4,6,7}},
        {"it", {1,9}},
        {"is", {1,5,8,9,10,12}},
        {"known", {9}},
        {"late", {1}},
        {"last", {10}},
        {"list", {5}},
        {"like", {13}},
        {"new", {11}},
        {"now", {1,9,11}},
        {"not", {12}},
        {"on", {2,3}},
        {"one", {11}},
        {"or", {4}},
        {"of", {2}},
        {"paths", {13}},
        {"program", {8}},
        {"probably", {12}},
        {"right", {3}},
        {"saying", {4}},
        {"sentence", {2,3,10}},
        {"should", {3,6,7,10,11,13}},
        {"space", {2}},
        {"start", {2,11}},
        {"symbol", {10}},
        {"to", {8}},
        {"too", {1}},
        {"that", {10}},
        {"the", {2,10}},
        {"this", {2,6,8,12}},
        {"yet", {12}},
        {"vulnerable", {8}},
    };

    return expectation;
}

TEST_F(ExtremeDatasetFixture, ExtremeSymbolsDataset)
{
    Concordance concordance = Concordance::makeFromFile(m_temp_file);
    std::map<Word, std::vector<Sentence> > expectation = getExpectationOfExtremeDataset();
    COMPARE_CONCORDANCE_WITH_EXPECTATION(concordance, expectation);
}