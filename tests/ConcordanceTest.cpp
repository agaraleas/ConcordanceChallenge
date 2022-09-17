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
    EXPECT_EQ(index, std::distance(EXPECTATION.begin(), current) + 1);                              \
    EXPECT_EQ(word, current->first);                                                                \
    EXPECT_EQ(occurrences.get(), current->second);                                                   \
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


class createConcordanceFromFileFixture : public ::testing::Test {
public:
    createConcordanceFromFileFixture() {}
    ~createConcordanceFromFileFixture() {}

    void SetUp() {
        m_temp_file = std::tmpnam(nullptr);

        std::string dataset = "Given an arbitrary text document written in English, write a program that will generate a \n";
        dataset += "concordance, i.e. an alphabetical list of all word occurrences, labeled with word \n";
        dataset += "frequencies. \n\n";
        dataset += "Bonus: label each word with the sentence numbers in which each occurrence appeared";

        std::ofstream out(m_temp_file);
        out << dataset;
        out.close();
    }

    void TearDown() {
        remove(m_temp_file.c_str());
    }

    std::string m_temp_file;
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
        {"each", {2}},
        {"english", {1}},
        {"frequencies", {1}},
        {"generate", {1}},
        {"given", {1}},
        {"i.e.", {1}},
        {"in", {1,2}},
        {"it", {2}},
        {"label", {2}},
        {"labeled", {1}},
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
        {"word", {1,1,2}},
        {"write", {1}},
        {"written", {1}},
    };

    return expectation;
}

TEST_F(createConcordanceFromFileFixture, DISABLED_ExampleDataset)
{
    Concordance concordance = Concordance::makeFromFile(m_temp_file);
    std::map<Word, std::vector<Sentence> > expectation = getExpectationOfExampleDataset();
    COMPARE_CONCORDANCE_WITH_EXPECTATION(concordance, expectation);
}
