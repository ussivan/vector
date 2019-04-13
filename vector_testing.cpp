#include <gtest/gtest.h>
#include "vector.h"

typedef my_vector container;

TEST(correctness, default_ctor)
{
    container c;
    EXPECT_EQ(0u, c.size());
}

