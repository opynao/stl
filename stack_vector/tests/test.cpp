#include "stack_vector.h"
#include "profile.h"

#include "gtest/gtest.h"

#include <vector>
#include <random>

TEST(TestConstruction, Test1)
{
    StackVector<int, 10> v;
    EXPECT_EQ(v.Size(), 0u);
    EXPECT_EQ(v.Capacity(), 10u);

    StackVector<int, 8> u(5);
    EXPECT_EQ(u.Size(), 5u);
    EXPECT_EQ(u.Capacity(), 8u);

    bool IsException{false};
    try
    {
        StackVector<int, 10> u(50);
    }
    catch (std::invalid_argument &)
    {
        IsException = true;
    }
    EXPECT_EQ(IsException, true);
}

TEST(TestPushBack, Test2)
{
    StackVector<int, 5> v;
    for (size_t i = 0; i < v.Capacity(); ++i)
        v.PushBack(i);

    bool IsException{false};
    try
    {
        v.PushBack(0);
    }
    catch (std::overflow_error &)
    {
        IsException = true;
    }
    EXPECT_EQ(IsException, true);
}

TEST(TestPopBack, Test3)
{
    StackVector<int, 5> v;
    for (size_t i = 1; i <= v.Capacity(); ++i)
        v.PushBack(i);

    for (int i = v.Size(); i > 0; --i)
        EXPECT_EQ(v.PopBack(), i);

    bool IsException{false};
    try
    {
        v.PopBack();
    }
    catch (std::underflow_error &)
    {
        IsException = true;
    }
    EXPECT_EQ(IsException, true);
}

constexpr size_t MAX_SIZE = 2500;

std::vector<std::vector<int>> generateTestData()
{
    std::default_random_engine re;
    std::uniform_int_distribution<int> value_gen(1, MAX_SIZE);

    std::vector<std::vector<int>> test_data(50000);

    for (auto &cur_vec : test_data)
    {
        cur_vec.resize(value_gen(re));

        for (int &x : cur_vec)
            x = value_gen(re);
    }
    return test_data;
}

// Stack vector is faster
TEST(Speed, Test1)
{
    auto test_data = generateTestData();
    auto f1 = [&test_data]()
    {
        for (auto &cur_vec : test_data)
        {
            StackVector<int, MAX_SIZE> v;
            for (const int &x : cur_vec)
                v.PushBack(x);
        }
    };
    auto f2 = [&test_data]()
    {
        for (auto &cur_vec : test_data)
        {
            std::vector<int> v;
            v.reserve(cur_vec.size());
            for (const int &x : cur_vec)
                v.push_back(x);
        }
    };
    auto f3 = [&test_data]()
    {
        for (auto &cur_vec : test_data)
        {
            std::vector<int> v;
            for (const int &x : cur_vec)
                v.push_back(x);
        }
    };
    std::cerr << "StackVector<int> runtime=" << profiler<>::duration(f1).count() << "ms\n";
    std::cerr << "std::vector<int> without reserve runtime=" << profiler<>::duration(f2).count() << "ms\n";
    std::cerr << "std::vector<int> with reserve runtime=" << profiler<>::duration(f3).count() << "ms\n";
}