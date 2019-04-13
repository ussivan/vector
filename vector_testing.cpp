#include <gtest/gtest.h>
#include "vector.h"
#include "fault_injection.h"

typedef my_vector container;

TEST(correctness, default_ctor)
{
    faulty_run([]
    {
        container c;
        EXPECT_EQ(0u, c.size());
        EXPECT_TRUE(c.empty());
    });
}

TEST(correctness, push_back_simple)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(1u, c.size());
        c.push_back(8);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(2u, c.size());
        c.push_back(15);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(3u, c.size());
        c.push_back(16);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(4u, c.size());
        c.push_back(23);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(5u, c.size());
        c.push_back(42);
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(6u, c.size());
    });
}

TEST(correctness, push_back_big)
{
    faulty_run([]
    {
        container c;
        
        for (size_t i = 0; i != 20; ++i)
        {
            int val = (i + 42) % 13;
            c.push_back(val);
        }

        for (size_t i = 0; i != 20; ++i)
        {
            int val = (i + 42) % 13;
            EXPECT_EQ(val, c[i]);
        }
    });
}

TEST(correctness, pop_back)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(5u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(4u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(3u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(2u, c.size());
        c.pop_back();
        EXPECT_FALSE(c.empty());
        EXPECT_EQ(1u, c.size());
        c.pop_back();
        EXPECT_TRUE(c.empty());
        EXPECT_EQ(0u, c.size());
    });
}

TEST(correctness, copy_ctor)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);

        container d = c;
        d[2] = 10;
        EXPECT_EQ(3, c[2]);

        d.push_back(4);
        EXPECT_EQ(3u, c.size());
        EXPECT_EQ(4u, d.size());
    });
}

TEST(correctness, copy_ctor_empty)
{
    faulty_run([]
    {
        container c;
        container d = c;
    });
}

TEST(correctness, assignment)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);

        container d;
        d.push_back(4);
        d.push_back(5);

        d = c;
        EXPECT_EQ(3u, d.size());
        EXPECT_EQ(1, d[0]);
        EXPECT_EQ(2, d[1]);
        EXPECT_EQ(3, d[2]);
    });
}

TEST(correctness, assignment_empty)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        container d;
        d = c;
    });
}

TEST(correctness, assignment_empty_2)
{
    faulty_run([]
    {
        container c;
        container d;
        d.push_back(1);
        d.push_back(2);
        d.push_back(3);
        d = c;
    });
}

TEST(correctness, assignment_self)
{
    faulty_run([]
    {
        container c;
        c.push_back(1);
        c.push_back(2);
        c.push_back(3);
        c = c;
    });
}

TEST(correctness, assignment_self_empty)
{
    faulty_run([]
    {
        container c;
        c = c;
    });
}

TEST(correctness, subscript)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);
        
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, insert)
{
    faulty_run([]
    {
        container c;
        c.insert(0, 15);
        c.insert(1, 42);
        c.insert(1, 16);
        c.insert(2, 23);
        c.insert(0, 4);
        c.insert(1, 8);

        EXPECT_EQ(6u, c.size());
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, erase)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(2);
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(16, c[2]);
        EXPECT_EQ(23, c[3]);
        EXPECT_EQ(42, c[4]);
    });
}

TEST(correctness, erase_begin)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(0);
        EXPECT_EQ(8, c[0]);
        EXPECT_EQ(15, c[1]);
        EXPECT_EQ(16, c[2]);
        EXPECT_EQ(23, c[3]);
        EXPECT_EQ(42, c[4]);
    });
}

TEST(correctness, erase_end)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(5);
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
    });
}

TEST(correctness, erase_range_begin)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(0, 2);
        EXPECT_EQ(15, c[0]);
        EXPECT_EQ(16, c[1]);
        EXPECT_EQ(23, c[2]);
        EXPECT_EQ(42, c[3]);
    });
}

TEST(correctness, erase_range_middle)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(2, 5);
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(42, c[2]);
    });
}

TEST(correctness, erase_range_end)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(3, 6);
        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
    });
}

TEST(correctness, erase_range_all)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        c.erase(0, 6);
        EXPECT_TRUE(c.empty());
    });
}

TEST(correctness, reserve)
{
    faulty_run([]
    {
        container c;
        c.reserve(10);
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);

        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}

TEST(correctness, reserve_2)
{
    faulty_run([]
    {
        container c;
        c.push_back(4);
        c.push_back(8);
        c.push_back(15);
        c.push_back(16);
        c.push_back(23);
        c.push_back(42);
        c.reserve(100);

        EXPECT_EQ(4, c[0]);
        EXPECT_EQ(8, c[1]);
        EXPECT_EQ(15, c[2]);
        EXPECT_EQ(16, c[3]);
        EXPECT_EQ(23, c[4]);
        EXPECT_EQ(42, c[5]);
    });
}
