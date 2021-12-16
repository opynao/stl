#include "vector.h"
#include "profile.h"

#include "gtest/gtest.h"

#include <vector>
#include <string>

class StringNonCopyable : public std::string
{
public:
    using std::string::string;

    StringNonCopyable(std::string &&other)
        : std::string(std::move(other))
    {
    }

    StringNonCopyable(const StringNonCopyable &) = delete;
    StringNonCopyable &operator=(const StringNonCopyable &) = delete;

    StringNonCopyable(StringNonCopyable &&) = default;
    StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

TEST(TestConstruction, Test1)
{
    Vector<int> empty;
    EXPECT_EQ(empty.Size(), 0u);
    EXPECT_EQ(empty.Capacity(), 0u);
    EXPECT_EQ(empty.begin(), empty.end());

    Vector<std::string> five_strings(5);
    EXPECT_EQ(five_strings.Size(), 5u);
    EXPECT_LE(five_strings.Size(), five_strings.Capacity());

    for (auto &item : five_strings)
        EXPECT_EQ(item.empty(), true);

    five_strings[2] = "Hello";
    EXPECT_EQ(five_strings[2], "Hello");
}

TEST(TestPushBack, Test2)
{
    Vector<int> v;
    for (int i = 10; i >= 1; --i)
    {
        v.PushBack(i);
        EXPECT_LE(v.Size(), v.Capacity());
    }
    std::sort(std::begin(v), std::end(v));

    const std::vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    EXPECT_EQ(v.Size(), expected.size());
    EXPECT_EQ(std::equal(std::begin(v), std::end(v), std::begin(expected)), true);
}

TEST(TestNoCopy, Test3)
{
    Vector<StringNonCopyable> strings;
    static const int SIZE = 10;

    for (int i = 0; i < SIZE; ++i)
        strings.PushBack(StringNonCopyable(std::to_string(i)));

    for (int i = 0; i < SIZE; ++i)
        EXPECT_EQ(strings[i], std::to_string(i));
}

// Vector<int> a bit slower than std::vector
constexpr int ELEMENTS_COUNT{100000000};
TEST(Speed, Test)
{
    auto f1 = []()
    {
        Vector<int> v;
        for (int i = 0; i != ELEMENTS_COUNT; ++i)
            v.PushBack(i);
    };
    auto f2 = []()
    {
        std::vector<int> v;
        for (int i = 0; i != ELEMENTS_COUNT; ++i)
            v.push_back(i);
    };
    std::cerr << "Vector<int> runtime=" << profiler<>::duration(f1).count() << "ms\n";
    std::cerr << "std::vector<int> runtime=" << profiler<>::duration(f2).count() << "ms\n";
}