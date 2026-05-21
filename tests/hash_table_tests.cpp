#include <string>
#include <gtest/gtest.h>
#include "open_address_hash_table.hpp"
TEST(HashTableTest, InitialStateIsCorrect) 
{
    OpenAddressHashTable<int, std::string> table;

    EXPECT_EQ(table.get_size(), 0);
    EXPECT_EQ(table.get_capacity(), 8);
}

TEST(HashTableTest, InsertAndGetSingleElement) 
{
    OpenAddressHashTable<int, std::string> table;

    table.insert(1, "One");
    auto result = table.get(1);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "One");
    EXPECT_EQ(table.get_size(), 1);
}

TEST(HashTableTest, GetReturnsNulloptForMissingKey) 
{
    OpenAddressHashTable<int, std::string> table;

    table.insert(1, "One");
    auto result = table.get(999);

    EXPECT_FALSE(result.has_value());
}

TEST(HashTableTest, UpdateExistingKeyOverwritesValue) 
{
    OpenAddressHashTable<int, std::string> table;

    table.insert(1, "OldValue");
    table.insert(1, "NewValue");
    auto result = table.get(1);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), "NewValue");
    EXPECT_EQ(table.get_size(), 1);
}

TEST(HashTableTest, RemoveDeletesElementAndDecreasesSize) 
{
    OpenAddressHashTable<int, std::string> table;

    table.insert(1, "One");
    table.insert(2, "Two");
    table.remove(1);
    auto result = table.get(1);

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(table.get_size(), 1);
}

TEST(HashTableTest, OperatorBracketCreatesAndReturnsReference) 
{
    OpenAddressHashTable<std::string, int> table;

    table["apples"] = 5;
    table["apples"] += 10;
    auto result = table.get("apples");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 15);
    EXPECT_EQ(table.get_size(), 1);
}

TEST(HashTableTest, RehashIncreasesCapacityAndKeepsData) 
{
    OpenAddressHashTable<int, int> table(8);

    for (int i = 0; i < 6; ++i) 
    {
        table.insert(i, i * 10);
    }
    size_t old_capacity = table.get_capacity();
    
    table.insert(6, 60); 

    EXPECT_GT(table.get_capacity(), old_capacity);
    EXPECT_EQ(table.get_capacity(), 16);
    EXPECT_EQ(table.get_size(), 7);
    for (int i = 0; i <= 6; ++i) 
    {
        auto result = table.get(i);
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), i * 10);
    }
}

TEST(HashTableTest, IteratorSkipsEmptyAndDeletedNodes) 
{
    OpenAddressHashTable<int, std::string> table;

    table.insert(1, "A");
    table.insert(2, "B");
    table.insert(3, "C");
    table.remove(2); 

    int count = 0;
    bool found_one = false;
    bool found_three = false;

    for (const auto& pair : table) 
    {
        count++;
        if (pair.first == 1 && pair.second == "A") found_one = true;
        if (pair.first == 3 && pair.second == "C") found_three = true;
    }

    EXPECT_EQ(count, 2);
    EXPECT_TRUE(found_one);
    EXPECT_TRUE(found_three);
}


TEST(HashTableStressTest, HandlesLargeAmountOfDataAndRehashes) 
{
    OpenAddressHashTable<int, int> table;
    const int num_elements = 20000000;

    for (int i = 0; i < num_elements; ++i) 
    {
        table.insert(i, i * 2);
    }

    EXPECT_EQ(table.get_size(), num_elements);
    EXPECT_GE(table.get_capacity(), num_elements); 

    auto result_first = table.get(0);
    auto result_mid = table.get(num_elements / 2);
    auto result_last = table.get(num_elements - 1);

    ASSERT_TRUE(result_first.has_value());
    EXPECT_EQ(result_first.value(), 0);

    ASSERT_TRUE(result_mid.has_value());
    EXPECT_EQ(result_mid.value(), (num_elements / 2) * 2);

    ASSERT_TRUE(result_last.has_value());
    EXPECT_EQ(result_last.value(), (num_elements - 1) * 2);
}

TEST(HashTableStressTest, HeavyOperatorBracketUsage) 
{
    OpenAddressHashTable<std::string, int> table;
    const int operations = 50000;

    for (int i = 0; i < operations; ++i) 
    {
        std::string key = "key_" + std::to_string(i % 1000); 
        table[key]++;
    }

    EXPECT_EQ(table.get_size(), 1000);
    EXPECT_EQ(table["key_0"], operations / 1000);
    EXPECT_EQ(table["key_999"], operations / 1000);
}
