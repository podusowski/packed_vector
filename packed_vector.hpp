#pragma once

#include <new>

template<class T>
struct metadata_t
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

    ~packed_vector()
    {
        if (_storage)
            operator delete(_storage);
    }

    packed_vector(std::initializer_list<T>&& values)
    {
        reallocate(values.size());
        std::copy(std::begin(values), std::end(values), begin());
    }

    //template<class It>
    //packed_vector(It&& begin, It&& end)
    //{
    //    std::copy(std::begin(values), std::end(values), std::back_inserter(*this));
    //}

    void push_back(T value)
    {
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
        void* st = ::operator new(new_size * sizeof(T) + sizeof(metadata_t<T>));
        _storage = new (st) metadata_t<T>{};
        _storage->_begin = new (next<metadata_t<T>>(_storage)) T[new_size];
        _storage->_end = std::next(_storage->_begin, new_size);
    }

    metadata_t<T>* _storage;
};

// the main reason why you might be interested in this class
static_assert(sizeof(packed_vector<int>) <= sizeof(void*));
