#pragma once

#include <iterator>

template<typename T>
struct vector {
    typedef T* iterator;
    typedef const T* const_iterator;

    vector() noexcept;

    vector(vector const &other);

    vector &operator=(vector const &other) {
        deleteAll();
        small = other.small;
        if(small) {
            try {
                new (&val) T(other.val);
            } catch (...) {
                small = false;
                buf = nullptr;
            }
            return *this;
        }
        buf = other.buf;
        if(buf)
            buf[REFS_I]++;
        return *this;
    }

    ~vector();

    T &operator[](size_t index);

    T const &operator[](size_t index) const noexcept;

    T &back();

    T const &back() const noexcept;

    T &front();

    T const &front() const noexcept;

    void push_back(T const value);

    void pop_back();

    iterator begin() noexcept {
        copy();
        if(small) {
            return &val;
        }
        if(buf == nullptr) {
            return nullptr;
        }
        return &(*this)[0];
    };

    const_iterator begin() const noexcept{
        if(small) {
            return &val;
        }
        if(buf == nullptr) {
            return nullptr;
        }
        return const_iterator (&(*this)[0]);
    }

    iterator end() noexcept {
        if(small) {
            return &val + 1;
        }
        if(buf == nullptr) {
            return nullptr;
        }
        return reinterpret_cast<T*>(buf + 3) + size();
    };

    const_iterator end() const noexcept {
        return const_iterator (end());
    };

    std::reverse_iterator<T*> rbegin() {
        return std::reverse_iterator<T*>(end());
    }

    std::reverse_iterator<T*> rend() {
        return std::reverse_iterator<T*>(begin())   ;
    }

    bool empty() const noexcept;

    size_t size() const noexcept;

    size_t capacity() const noexcept;

    void copy();

    void reserve(size_t n);

    void shrink_to_fit();

    void resize(size_t n, T const &val = T());

    void clear();

    iterator insert(const_iterator pos, T const &val) {
        copy();
        size_t i = 0;
        iterator j = begin();
        while(j != pos) {
            j++;
            i++;
        }
        auto *newBuf = reinterpret_cast<size_t*>(operator new [](3 * sizeof(size_t) + (size() + 1) * sizeof(T)));
        newBuf[SIZE_I] = size() + 1;
        newBuf[CAP_I] = size() + 1;
        newBuf[REFS_I] = 1;
        auto pointer = reinterpret_cast<T*>(newBuf + 3);
        size_t l;
        try {
            for (l = 0; l < i; l++, pointer++) {
                new(pointer) T((*this)[l]);
            }
            new(pointer) T(val);
            pointer++;
            for (l = i + 1; l <= size(); l++, pointer++) {
                new(pointer) T((*this)[l - 1]);
            }
            deleteAll();
            buf = newBuf;
        } catch (...) {
            for(size_t k = 0; k < l; k++) {
                (*(reinterpret_cast<T*>(newBuf + 3) + k)).~T();
            }
            delete[] newBuf;
            throw;
        }
        return begin() + i;
    }

    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }

    iterator erase(const_iterator first, const_iterator last) {
        copy();
        if(last - first <= 0) {
            return begin();
        }
        if(static_cast<size_t>(last - first) == size()) {
            deleteAll();
            return begin();
        }
        size_t i = 0;
        iterator j = begin();
        while(j != first) {
            i++;
            j++;
        }
        auto *newBuf = reinterpret_cast<size_t*>
                (operator new [](3 * sizeof(size_t) + (size() - (last - first)) * sizeof(T)));
        newBuf[SIZE_I] = size() - (last - first);
        newBuf[CAP_I] = size() - (last - first);
        newBuf[REFS_I] = 1;
        auto pointer = reinterpret_cast<T*>(newBuf + 3);
        size_t l;
        try {
            for (l = 0; l < i; l++, pointer++) {
                new(pointer) T((*this)[l]);
            }
            while (j != last) {
                j++;
                l++;
            }
            for (; l < size(); l++, pointer++) {
                new(pointer) T((*this)[l]);
            }
            deleteAll();
            buf = newBuf;
        } catch (...) {
            for(size_t k = 0; k < l; k++) {
                (*(reinterpret_cast<T*>(newBuf + 3) + k)).~T();
            }
            delete[] newBuf;
            throw;
        }
        return begin() + i;
    }
    template <typename P>
    friend bool operator <(vector<P> const &a, vector<P> const &b) noexcept {
        for(size_t i = 0; i < std::min(a.size(), b.size()); i++) {
            if(a[i] != b[i]) {
                return a[i] < b[i];
            }
        }
        return a.size() < b.size();
    }
    template <typename P>
    friend bool operator >(vector<P> const &a, vector<P> const &b) noexcept{
        return b < a;
    }
    template <typename P>
    friend bool operator <=(vector<P> const &a, vector<P> const &b) noexcept{
        return !(a > b);
    }
    template <typename P>
    friend bool operator >=(vector<P> const &a, vector<P> const &b) noexcept{
        return !(a < b);
    }
    template <typename P>
    friend void swap(vector<P> const &a, vector<P> const &b) {
        if(a.small && b.small) {
            std::swap(a.val, b.val);
            return;
        }
        if(a.small) {
            vector<P> t(b);
            b.deleteAll();
            try {
                new(&b.val) P(a.val);
            } catch (...) {
                b.buf = t.buf;
                throw;
            }
            b.small = true;
            a.small = false;
            a.val.~P();
            a.buf = t.buf;
            return;
        }
        if(b.small) {
            vector<P> t(a);
            a.deleteAll();
            try {
                new(&a.val) P(b.val);
            } catch (...) {
                a.buf = t.buf;
                throw;
            }
            a.small = true;
            b.small = false;
            b.val.~P();
            b.buf = t.buf;
            return;
        }
        std::swap(a.buf, b.buf);
    }

private:
    static size_t const SIZE_I = 0;
    static size_t const CAP_I = 1;
    static size_t const REFS_I = 2;
    bool small;
    union {
        T val;
        size_t *buf;
    };

    void expand(size_t n) {
        copy();
        size_t sz = empty() ? 0 : small ? 1 : size();
        if(n == 1) {
            vector v(*this);
            if(sz > 0) {
                T t(front());
                deleteAll();
                small = true;
                try {
                    new(&val) T(t);
                } catch (...) {
                    *this = v;
                    throw;
                }
                return;
            }
            deleteAll();
            return;
        }
        auto *newBuf = reinterpret_cast<size_t*>(operator new [](3 * sizeof(size_t) + n * sizeof(T)));
        newBuf[SIZE_I] = sz;
        newBuf[CAP_I] = n;
        newBuf[REFS_I] = 1;
        T* t = reinterpret_cast<T*> (newBuf + 3);
        size_t i;
        try {
            for (size_t i = 0; i < sz; i++) {
                new(t + i) T((*this)[i]);
            }
        } catch (...) {
            for(size_t k = 0; k < i; k++) {
                (*(reinterpret_cast<T*>(newBuf + 3) + k)).~T();
            }
            delete[] newBuf;
            throw;
        }
        deleteAll();
        buf = newBuf;
    }

    void deleteAll() {
        if(small) {
            val.~T();
        } else {
            if(buf == nullptr) {
                return;
            }
            for(size_t i = 0; i < size(); i++) {
                (*this)[i].~T();
            }
            delete[] buf;
        }
        small = false;
        buf = nullptr;
    }
};


template<typename T>
vector<T>::vector() noexcept {
    small = false;
    buf = nullptr;
}

template<typename T>
void vector<T>::push_back(const T value) {
    copy();
    if (empty()) {
        try {
            new (&val) T(value);
        } catch (...) {
            buf = nullptr;
            small = false;
            throw;
        }
        small = true;
        return;
    }
    size_t n = small ? 1 : size();
    if(small || n == capacity()) {
        expand(2 * n);
    }
    try {
        new (&(*this)[n]) T(value);
    } catch (...) {
       expand(n);
       throw;
    }
    buf[SIZE_I]++;
}

template<typename T>
bool vector<T>::empty() const noexcept {
    return !small && !buf;
}

template<typename T>
vector<T>::~vector() {
    copy();
    deleteAll();
}

template<typename T>
void vector<T>::clear() {
    copy();
    deleteAll();
}

template<typename T>
size_t vector<T>::size() const noexcept {
    if (empty()) {
        return 0;
    }
    if (small) {
        return 1;
    }
    return buf[SIZE_I];
}

template<typename T>
size_t vector<T>::capacity() const noexcept {
    if(empty()) {
        return 0;
    }
    if(small) {
        return 1;
    }
    return buf[CAP_I];
}

template<typename T>
T &vector<T>::operator[](size_t index) {
    copy();
    if(small) {
        return val;
    }
    return *(reinterpret_cast<T*>(buf + 3) + index);
}

template<typename T>
T const &vector<T>::operator[](size_t index) const noexcept {
    if(small) {
        return val;
    }
    return *(reinterpret_cast<T*>(buf + 3) + index);
}

template<typename T>
T &vector<T>::front() {
    copy();
    return (*this)[0];
}

template<typename T>
void vector<T>::reserve(size_t n) {
    copy();
    if(n <= 1 || n < capacity()) {
        return;
    }
    expand(n);
}

template<typename T>
void vector<T>::resize(size_t n, const T &val) {
    expand(n);
    while(size() < n) {

    }
}

template<typename T>
T &vector<T>::back() {
    copy();
    return (*this)[size() - 1];
}

template<typename T>
void vector<T>::pop_back() {
    copy();
    if(size() <= 1) {
        deleteAll();
        return;
    }
    (*this)[size() - 1].~T();
    buf[SIZE_I]--;
    if(size() == 1) {
        vector v(*this);
        T value((*this)[0]);
        deleteAll();
        try {
            new(&val) T(value);
        } catch (...) {
            *this = v;
            throw;
        }
        small = true;
    }
}

template<typename T>
T const &vector<T>::back() const noexcept{
    return (*this)[size() - 1];
}

template<typename T>
vector<T>::vector(vector const &other) {
    small = other.small;
    if(small) {
        try {
            new (&val) T(other.val);
        } catch (...) {
            small = false;
            buf = nullptr;
            throw;
        }
        return;
    }
    buf = other.buf;
    if(buf)
        buf[REFS_I]++;
}

template<typename T>
T const &vector<T>::front() const noexcept {
    return (*this)[0];
}

template<typename T>
void vector<T>::shrink_to_fit() {
    if(size() <= 1) {
        return;
    }
    expand(size());
}

template<typename T>
void vector<T>::copy() {
    if(small || buf == nullptr || buf[REFS_I] == 1) {
        return;
    }
    auto *newBuf = reinterpret_cast<size_t*>(operator new [](3 * sizeof(size_t) + capacity() * sizeof(T)));
    newBuf[SIZE_I] = buf[SIZE_I];
    newBuf[CAP_I] = buf[CAP_I];
    newBuf[REFS_I] = 1;
    buf[REFS_I]--;
    size_t i;
    auto pointer = reinterpret_cast<T*>(newBuf + 3);
    try {
        for (i = 0; i < size(); i++, pointer++) {
            new(pointer) T((*this)[i]);
        }
    } catch (...) {
        for(size_t k = 0; k < i; k++) {
            (*(reinterpret_cast<T*>(newBuf + 3) + k)).~T();
        }
        delete[] newBuf;
        throw;
    }
    buf = newBuf;
}
