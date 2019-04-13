#include "counted.h"
#include <gtest/gtest.h>

#include "fault_injection.h"

counted::counted(int data)
    : data(data)
{
    fault_injection_point();
    fault_injection_disable fd;
    auto p = instances.insert(this);
    EXPECT_TRUE(p.second);
}

counted::counted(counted const& other)
    : data(other.data)
{
    fault_injection_point();
    fault_injection_disable fd;
    auto p = instances.insert(this);
    EXPECT_TRUE(p.second);
}

counted::~counted()
{
    fault_injection_disable fd;
    size_t n = instances.erase(this);
    EXPECT_EQ(1u, n);
}

counted& counted::operator=(counted const& c)
{
    fault_injection_point();
    {
        fault_injection_disable fd;
        EXPECT_TRUE(instances.find(this) != instances.end());
    }

    data = c.data;
    return *this;
}

counted::operator int() const
{
    fault_injection_disable fd;
    EXPECT_TRUE(instances.find(this) != instances.end());

    return data;
}

std::set<counted const*> counted::instances;

counted::no_new_instances_guard::no_new_instances_guard()
    : old_instances(instances)
{}

counted::no_new_instances_guard::~no_new_instances_guard()
{
    fault_injection_disable fd;
    EXPECT_TRUE(old_instances == instances);
}

void counted::no_new_instances_guard::expect_no_instances()
{
    fault_injection_disable fd;
    EXPECT_TRUE(old_instances == instances);
}
