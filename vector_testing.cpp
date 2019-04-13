#include <gtest/gtest.h>
#include "vector.h"

typedef my_vector container;

TEST(correctness, default_ctor)
{
    container c;
    EXPECT_EQ(0u, c.size());
    EXPECT_TRUE(c.empty());
}

TEST(correctness, push_back)
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
