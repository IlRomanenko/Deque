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

TEST_F(DequeTest, VectorInit)
{
    for (int i = 0; i < 100; i++)
        deque_int.push_back(2 * i + 1);
    vector<int> v(deque_int.begin(), deque_int.end());
    
    sort(v.rbegin(), v.rend());
    for (int i = 0; i < 300; i++)
        deque_int.push_back(2 * i + 1);

    cout << v[0] << endl;
}

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
    chrono::steady_clock clock;

    int maxn = 1000 * 1000;

    vector<int> vector_int;

    for (int size = 10; size <= maxn; size *= 10)
    {
        auto before_deque_push = clock.now();
        fori(i, size)
            deque_int.push_back(random(engine));
        while (!deque_int.empty())
            deque_int.pop_back();
        auto deque_after_pop_back = clock.now();

        

        auto before_vector_push = clock.now();
        fori(i, size)
            vector_int.push_back(random(engine));
        while (!deque_int.empty())
            vector_int.pop_back();
        auto vector_after_pop_back = clock.now();

        auto deque_duration = deque_after_pop_back - before_deque_push;
        auto vector_duration = vector_after_pop_back - before_vector_push;

        cerr << endl;
        cerr << "Size = " << to_string(size) << endl;
        cerr << "deque_duration / vector_duration = " << deque_duration.count() / (double)vector_duration.count() << endl;
        cerr << "deque_time = " << deque_duration.count() / (1000 * 1000.0) << " ms" << endl;
        cerr << "vector_time = " << vector_duration.count() / (1000 * 1000.0) << " ms" << endl;
        cerr << "relative_time = " << deque_duration.count() / (double)size / (1000 * 1000.0) << " ms" << endl;
        cerr << endl;
    }
    cerr << endl;
}

TEST_F(DequeTest, ReverseDeque_small)
{
    deque_int.push_back(10);
    deque_int.push_back(20);
    deque_int.push_front(30);

    //30, 10, 20
    reverse(deque_int.begin(), deque_int.end());
    //20, 10, 30

    const int maxn = 200;
    fori(i, maxn)
        deque_int.push_back(i * 2 + 1);

    //20, 10, 30, 1, 3, 5, 7, 9, ...
    reverse(deque_int.begin(), deque_int.end());

    // ..., 9, 7, 5, 3, 1, 30, 10, 20
    fori(i, maxn)
    {
        EXPECT_EQ(i * 2 + 1, deque_int[maxn - i - 1]);
    }
}

TEST_F(DequeTest, SortDeque)
{
    const int maxn = 1000 * 1000;

    for (int size = 10; size < maxn; size *= 10)
    {
        int elem;
        vector<int> temp_v;
        fori(i, size)
        {
            elem = random(engine);
            deque_int.push_back(elem);
            temp_v.push_back(elem);
        }

        EXPECT_EQ(temp_v.size(), deque_int.size());

        chrono::steady_clock clock;

        auto before_vector_sort = clock.now();
        sort(temp_v.begin(), temp_v.end());
        auto after_vector_sort = clock.now();

        auto before_deque_sort = clock.now();
        sort(deque_int.begin(), deque_int.end());
        auto after_deque_sort = clock.now();

        auto deque_duration = after_deque_sort - before_deque_sort;
        auto vector_duration = after_vector_sort - before_vector_sort;

        cerr << endl;
        cerr << "Size = " << to_string(size) << endl;
        cerr << "deque_duration / vector_duration = " << deque_duration.count() / (double)vector_duration.count() << endl;
        cerr << "deque_time = " << deque_duration.count() / (1000 * 1000.0) << " ms" << endl;
        cerr << "vector_time = " << vector_duration.count() / (1000 * 1000.0) << " ms" << endl;
        cerr << endl;
        

        EXPECT_EQ(temp_v.size(), deque_int.size());

        while (!temp_v.empty() && !deque_int.empty())
        {
            EXPECT_EQ(temp_v.back(), deque_int.back());
            temp_v.pop_back();
            deque_int.pop_back();
        }
        EXPECT_EQ(temp_v.size(), deque_int.size());
    }
    cerr << endl;
}

TEST_F(DequeTest, SortDeque_ReverseOrder)
{
    const int maxn = 1000 * 1000;

    for (int size = 10; size < maxn; size *= 10)
    {
        int elem;
        vector<int> temp_v;
        fori(i, size)
        {
            elem = random(engine);
            deque_int.push_back(elem);
            temp_v.push_back(elem);
        }

        EXPECT_EQ(temp_v.size(), deque_int.size());

        chrono::steady_clock clock;

        auto before_vector_sort = clock.now();
        sort(temp_v.rbegin(), temp_v.rend());
        auto after_vector_sort = clock.now();

        auto before_deque_sort = clock.now();
        sort(deque_int.rbegin(), deque_int.rend());
        auto after_deque_sort = clock.now();

        auto deque_duration = after_deque_sort - before_deque_sort;
        auto vector_duration = after_vector_sort - before_vector_sort;

        cerr << endl;
        cerr << "Size = " << to_string(size) << endl;
        cerr << "deque_duration / vector_duration = " << deque_duration.count() / (double)vector_duration.count() << endl;
        cerr << "deque_time = " << deque_duration.count() / (1000*1000.0) << " ms" << endl;
        cerr << "vector_time = " << vector_duration.count() / (1000 * 1000.0) << " ms" << endl;
        cerr << endl;

        EXPECT_EQ(temp_v.size(), deque_int.size());

        while (!temp_v.empty() && !deque_int.empty())
        {
            EXPECT_EQ(temp_v.back(), deque_int.back());
            temp_v.pop_back();
            deque_int.pop_back();
        }
        EXPECT_EQ(temp_v.size(), deque_int.size());
    }
    cerr << endl;
}

TEST_F(DequeTest, LinearTime_1e6_clear_inside)
{
    chrono::steady_clock clock;

    int maxn = 1000 * 1000;

    vector<int> vector_int;

    for (int size = 10; size <= maxn; size *= 10)
    {
        auto before_deque_push = clock.now();
        fori(i, size)
            deque_int.push_back(random(engine));
        deque_int.clear();
        auto deque_after_pop_back = clock.now();



        auto before_vector_push = clock.now();
        fori(i, size)
            vector_int.push_back(random(engine));
        vector_int.clear();
        auto vector_after_pop_back = clock.now();

        auto deque_duration = deque_after_pop_back - before_deque_push;
        auto vector_duration = vector_after_pop_back - before_vector_push;

        cerr << endl;
        cerr << "Size = " << to_string(size) << endl;
        cerr << "deque_duration / vector_duration = " << deque_duration.count() / (double)vector_duration.count() << endl;
        cerr << "deque_time = " << deque_duration.count() / (1000 * 1000.0) << " ms" << endl;
        cerr << "vector_time = " << vector_duration.count() / (1000 * 1000.0) << " ms" << endl;
        cerr << "relative_time = " << deque_duration.count() / (double)size / (1000 * 1000.0) << " ms" << endl;
        cerr << endl;
    }
    cerr << endl;
}

int main(int argc, char **argv)
{
    cerr.setf(cerr.fixed);
    cerr.precision(9);

    testing::InitGoogleTest(&argc, argv);

    RUN_ALL_TESTS();

    system("pause");
    return 0;
}