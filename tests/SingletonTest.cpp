#include <gtest/gtest.h>
#include "Singleton.hpp"

class Dummy : public Singleton<Dummy>
{
public:
    friend class Singleton<Dummy>;

private:
    Dummy(){};
};

TEST(Singleton, Uniqueness)
{
    Dummy *call1 = Dummy::instance();
    Dummy *call2 = Dummy::instance();
    EXPECT_EQ(call1, call2);
}