#pragma once

#include <vector>
#include "counted.h"

struct my_vector
{
    typedef counted value_type;

    my_vector() = default;
    my_vector(my_vector const&) = default;
    my_vector& operator=(my_vector const&) = default;

    size_t size() const
    {
        return v.size();
    }

    value_type& operator[](size_t index)
    {
        return v[index];
    }

    value_type const& operator[](size_t index) const
    {
        return v[index];
    }

    void insert(size_t position, value_type const& value)
    {
        v.insert(v.begin() + position, value);
    }

    void erase(size_t position)
    {
        v.erase(v.begin() + position);
    }

    void erase(size_t first, size_t last)
    {
        v.erase(v.begin() + first, v.begin() + last);
    }

    bool empty() const
    {
        return v.empty();
    }

    value_type& back()
    {
        return v.back();
    }

    value_type const& back() const
    {
        return v.back();
    }

    void push_back(value_type const& value)
    {
        v.push_back(value);
    }
    
    void pop_back()
    {
        v.pop_back();
    }

    value_type& front()
    {
        return v.back();
    }

    value_type const& front() const
    {
        return v.back();
    }

    size_t capacity() const
    {
        return v.capacity();
    }

    void reserve(size_t desired)
    {
        v.reserve(desired);
    }

    void shrink_to_fit()
    {
        v.shrink_to_fit();
    }

    friend void swap(my_vector& a, my_vector& b)
    {
        using std::swap;
        swap(a.v, b.v);
    }

private:
    std::vector<value_type> v;
};

