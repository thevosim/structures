#include <string>
#include <map>
#include <random>
#include <gtest/gtest.h>
#include "unordered_table.hpp"

class UnorderedTableTests : public ::testing::Test 
{
};

TEST_F(UnorderedTableTests, BasicInsertAndFind)
{
    UnorderedTable<int, std::string> map;
    
    auto result = map.insert({1, "one"});
    auto it = map.find(1);

    EXPECT_TRUE(result.second);
    ASSERT_NE(it, map.end());
    EXPECT_EQ(it->second, "one");
    EXPECT_EQ(map.size(), 1);
}

TEST_F(UnorderedTableTests, DuplicateInsert)
{
    UnorderedTable<int, std::string> map;
    
    map.insert({1, "one"});
    auto result = map.insert({1, "duplicate"});

    EXPECT_FALSE(result.second);
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(map.find(1)->second, "one");
}

TEST_F(UnorderedTableTests, OperatorSquareBrackets)
{
    UnorderedTable<int, int> map;
    
    map[10] = 100;
    map[10] = 200;
    int value = map[10];
    int newValue = map[20];

    EXPECT_EQ(value, 200);
    EXPECT_EQ(newValue, 0);
    EXPECT_EQ(map.size(), 2);
}

TEST_F(UnorderedTableTests, EraseByKeyWithSwapLogic)
{
    UnorderedTable<int, std::string> map;
    map.insert({1, "first"});
    map.insert({2, "second"});
    map.insert({3, "third"});

    bool removed = map.erase(1);
    auto it2 = map.find(2);
    auto it3 = map.find(3);

    EXPECT_TRUE(removed);
    EXPECT_EQ(map.size(), 2);
    EXPECT_NE(it2, map.end());
    EXPECT_NE(it3, map.end());
}

TEST_F(UnorderedTableTests, EraseByIterator)
{
    UnorderedTable<int, int> map;
    map.insert({1, 10});
    map.insert({2, 20});
    auto it = map.find(1);

    auto nextIt = map.erase(it);

    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(map.begin()->first, 2);
    EXPECT_EQ(nextIt, map.begin());
}

TEST_F(UnorderedTableTests, ClearAndEmpty)
{
    UnorderedTable<int, int> map;
    map.insert({1, 10});

    map.clear();

    EXPECT_TRUE(map.empty());
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(map.find(1), map.end());
}

TEST_F(UnorderedTableTests, StressTestLinearGrowth)
{
    UnorderedTable<int, int> map;
    const int count = 10000;
    map.reserve(count);

    for (int i = 0; i < count; ++i)
    {
        map.push_back_unsafe({i, i * 2});
    }

    bool allFound = true;
    for (int i = 0; i < count; ++i)
    {
        if (map.find(i) == map.end())
        {
            allFound = false;
            break;
        }
    }

    EXPECT_EQ(map.size(), count);
    EXPECT_TRUE(allFound);
}

TEST_F(UnorderedTableTests, StressTestMassiveErase)
{
    UnorderedTable<int, int> map;
    const int count = 5000;
    for (int i = 0; i < count; ++i)
    {
        map.push_back_unsafe({i, i});
    }

    for (int i = 0; i < count; i += 2)
    {
        map.erase(i);
    }

    EXPECT_EQ(map.size(), count / 2);
    EXPECT_EQ(map.find(0), map.end());
    EXPECT_NE(map.find(1), map.end());
}