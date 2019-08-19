#pragma once

#include <new>

template<class T>
struct header
{
    T* _begin;
    T* _end;
};

template<class T>
struct packed_vector
{
    using iterator = T*;

    packed_vector() : _storage(nullptr)
    {
    }

    packed_vector(std::initializer_list<T>&& values)
    {
        reallocate(values.size());
        std::copy(std::begin(values), std::end(values), begin());
    }

    iterator begin()
    {
        if (_storage)
            return _storage->_begin;
        return nullptr;
    }

    iterator end()
    {
        if (_storage)
            return _storage->_end;
        return nullptr;
    }

private:
    template<class E>
    void* next(void* p)
    {
        return std::next(reinterpret_cast<E*>(p));
    }

    void reallocate(std::size_t new_size)
    {
        void* st = ::operator new(new_size * sizeof(T) + sizeof(header<T>));
        _storage = new (st) header<T>{};
        _storage->_begin = new (next<header<T>>(_storage)) T[new_size];
        _storage->_end = std::next(_storage->_begin, new_size);
    }

    header<T>* _storage;
};

// the main reason why you might be interested in this class
static_assert(sizeof(packed_vector<int>) <= sizeof(void*));
