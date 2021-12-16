#include "hash_table.h"
#include "profile.h"

#include "gtest/gtest.h"

#include <iterator>
#include <unordered_set>

struct IntHasher
{
    size_t operator()(int value) const
    {
        return value;
    }
};

struct TestValue
{
    int value;

    bool operator==(TestValue other) const
    {
        return value / 2 == other.value / 2;
    }
};

struct TestValueHasher
{
    size_t operator()(TestValue value) const
    {
        return value.value / 2;
    }
};

TEST(Basic, Smoke)
{
    HashTable<int, IntHasher> hash_set(2);
    hash_set.Add(3);
    hash_set.Add(4);

    EXPECT_EQ(hash_set.Has(3), true);
    EXPECT_EQ(hash_set.Has(4), true);
    EXPECT_EQ(!hash_set.Has(5), true);

    hash_set.Erase(3);

    EXPECT_EQ(!hash_set.Has(3), true);
    EXPECT_EQ(hash_set.Has(4), true);
    EXPECT_EQ(!hash_set.Has(5), true);

    hash_set.Add(3);
    hash_set.Add(5);

    EXPECT_EQ(hash_set.Has(3), true);
    EXPECT_EQ(hash_set.Has(4), true);
    EXPECT_EQ(hash_set.Has(5), true);
}

TEST(Basic, Empty)
{
    HashTable<int, IntHasher> hash_set(10);
    for (int value = 0; value < 10000; ++value)
    {
        EXPECT_EQ(!hash_set.Has(value), true);
    }
}

TEST(Basic, Idempotency)
{
    HashTable<int, IntHasher> hash_set(10);
    hash_set.Add(5);
    EXPECT_EQ(hash_set.Has(5), true);
    hash_set.Add(5);
    EXPECT_EQ(hash_set.Has(5), true);
    hash_set.Erase(5);
    EXPECT_EQ(!hash_set.Has(5), true);
    hash_set.Erase(5);
    EXPECT_EQ(!hash_set.Has(5), true);
}

TEST(Basic, Equivalence)
{
    HashTable<TestValue, TestValueHasher> hash_set(10);
    hash_set.Add(TestValue{2});
    hash_set.Add(TestValue{3});

    EXPECT_EQ(hash_set.Has(TestValue{2}), true);
    EXPECT_EQ(hash_set.Has(TestValue{3}), true);

    const auto &bucket = hash_set.GetBucket(TestValue{2});
    const auto &three_bucket = hash_set.GetBucket(TestValue{3});
    EXPECT_EQ(&bucket, &three_bucket);
    EXPECT_EQ(1, std::distance(begin(bucket), end(bucket)));
    EXPECT_EQ(2, bucket.front().value);
}

constexpr int ELEMENTS_COUNT = 100000;

// unordered multiset is faster than HashTable
TEST(Speed, Test)
{
    auto f1 = []()
    {
        HashTable<int, IntHasher> hash_table(1000);
        for (int i = 0; i != ELEMENTS_COUNT; ++i)
        {
            hash_table.Add(i);
            hash_table.Add(i);
        }
        for (int i = 0; i != ELEMENTS_COUNT; ++i)
        {
            hash_table.Erase(i);
            hash_table.Has(i);
        }
    };
    auto f2 = []()
    {
        std::unordered_multiset<int, IntHasher> hash_table(1000);
        for (int i = 0; i != ELEMENTS_COUNT; ++i)
        {
            hash_table.insert(i);
            hash_table.insert(i);
        }
        for (int i = 0; i != ELEMENTS_COUNT; ++i)
        {
            hash_table.erase(i);
            hash_table.find(i);
        }
    };
    std::cerr << "HashTable<int> runtime=" << profiler<>::duration(f1).count() << "ms\n";
    std::cerr << "std::unordered_set<int> runtime=" << profiler<>::duration(f2).count() << "ms\n";
}