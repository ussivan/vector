#include <gtest/gtest.h>
#include "vector.h"

typedef my_vector container;

TEST(correctness, default_ctor)
{
    container c;
    EXPECT_EQ(0u, c.size());
    EXPECT_TRUE(c.empty());
}

TEST(correctness, push_back_simple)
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
}

TEST(correctness, push_back_big)
{
    container c;
    
    for (size_t i = 0; i != 1000; ++i)
    {
        int val = (i + 42) % 13;
        c.push_back(val);
    }

    for (size_t i = 0; i != 1000; ++i)
    {
        int val = (i + 42) % 13;
        EXPECT_EQ(val, c[i]);
    }
}
