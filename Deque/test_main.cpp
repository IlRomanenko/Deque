#include <vld.h>
#include <gtest/gtest.h>
#include <random>
#include <chrono>

#include "base.h"
#include "deque.h"

class DequeTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {

    }

    virtual void TearDown()
    {
        deque_int.clear();
    }

    Deque<int> deque_int;
public:

    uniform_int_distribution<int> random;
    default_random_engine engine;

    void AddElements(int size)
    {
        fori(i, size)
            deque_int.push_back(random(engine));
    }
};

TEST_F(DequeTest, EmptyInitializer)
{
    EXPECT_EQ(0, deque_int.size());
    for_each(deque_int.begin(), deque_int.end(), 
        [](int elem) 
    {
        EXPECT_EQ(0, elem);
    });
}

TEST_F(DequeTest, ExtendCapacity)
{
    fori(i, 7)
        deque_int.push_back(i);
    deque_int.push_back(42);
    EXPECT_EQ(8, deque_int.size());
}

TEST_F(DequeTest, Can_PushBack_1e3)
{
    const int maxn = 1000;
    fori(i, maxn)
    {
        deque_int.push_back(random(engine));
        EXPECT_EQ(i + 1, deque_int.size());
    }

    EXPECT_EQ(1000, deque_int.size());
}

TEST_F(DequeTest, Can_PopBack_1e3)
{
    const int maxn = 1000;
    AddElements(maxn);
    fori(i, maxn)
    {
        deque_int.pop_back();
        EXPECT_EQ(maxn - i - 1, deque_int.size());
    }
}

TEST_F(DequeTest, Correct_PushBack_1e3)
{
    vector<int> v;
    const int maxn = 1000;
    int elem;
    fori(i, maxn)
    {
        elem = random(engine);
        v.push_back(elem);
        deque_int.push_back(elem);
    }

    fori(i, maxn)
    {
        EXPECT_EQ(v[i], deque_int[i]);
    }
}

TEST_F(DequeTest, Correct_PushFront_1e3)
{
    vector<int> v;
    const int maxn = 1000;
    int elem;
    fori(i, maxn)
    {
        elem = random(engine);
        v.push_back(elem);
        deque_int.push_front(elem);
    }

    fori(i, maxn)
    {
        EXPECT_EQ(v[maxn - i - 1], deque_int[i]);
    }
}

TEST_F(DequeTest, Correct_iterator_1e3)
{
    vector<int> v;
    const int maxn = 1000;
    int elem;
    fori(i, maxn)
    {
        elem = random(engine);
        v.push_back(elem);
        deque_int.push_back(elem);
    }
    EXPECT_EQ(v.size(), deque_int.size());

    int pos = 0;
    for (auto it = deque_int.begin(); it != deque_int.end(); it++, pos++)
        EXPECT_EQ(v[pos], *it);

    pos = 0;
    for (auto it = deque_int.cbegin(); it != deque_int.cend(); it++, pos++)
        EXPECT_EQ(v[pos], *it);
}

TEST_F(DequeTest, Correct_reverse_iterator_1e3)
{
    vector<int> v;
    const int maxn = 1000;
    int elem;
    fori(i, maxn)
    {
        elem = random(engine);
        v.push_back(elem);
        deque_int.push_back(elem);
    }
    EXPECT_EQ(v.size(), deque_int.size());

    int pos = (int)v.size() - 1;
    for (auto it = deque_int.rbegin(); it != deque_int.rend(); it++, pos--)
        EXPECT_EQ(v[pos], *it);
    
    pos = (int)v.size() - 1;
    for (auto it = deque_int.crbegin(); it != deque_int.crend(); it++, pos--)
        EXPECT_EQ(v[pos], *it);
}

TEST_F(DequeTest, Correct_PopBack_1e3)
{
    vector<int> v;
    const int maxn = 1000;
    int elem;
    fori(i, maxn)
    {
        elem = random(engine);
        v.push_back(elem);
        deque_int.push_back(elem);
    }

    fori(i, maxn)
    {
        EXPECT_EQ(deque_int.back(), v.back());
        deque_int.pop_back();
        v.pop_back();
    }
}

TEST_F(DequeTest, Correct_PopFront_1e3)
{
    vector<int> v;
    const int maxn = 1000;
    int elem;
    fori(i, maxn)
    {
        elem = random(engine);
        v.push_back(elem);
        deque_int.push_back(elem);
    }

    fori(i, maxn)
    {
        EXPECT_EQ(deque_int.front(), v[i]);
        deque_int.pop_front();
    }
}

TEST_F(DequeTest, Correct_clear)
{
    const int maxn = 1000;
    AddElements(maxn);
    deque_int.clear();
    EXPECT_EQ(0, deque_int.size());
}

TEST_F(DequeTest, Correct_size)
{
    const int maxn = 1000;
    AddElements(maxn);

    int count = 0;
    while (!deque_int.empty())
    {
        EXPECT_EQ(maxn - count, deque_int.size());
        deque_int.pop_back();
        count++;
    }
    EXPECT_EQ(maxn, count);
}

TEST_F(DequeTest, LinearTime_1e6)
{

    const int maxn = 1000 * 1000;
    fori(i, maxn)
        deque_int.push_back(random(engine));
    while (!deque_int.empty())
        deque_int.pop_back();
    
    EXPECT_EQ(0, deque_int.size());

    fori(i, maxn)
        deque_int.push_front(random(engine));
    while (!deque_int.empty())
        deque_int.pop_front();

    EXPECT_EQ(0, deque_int.size());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();

    system("pause");
    return 0;
}