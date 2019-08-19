#pragma once

#include <new>

template<class T>
struct packed_vector
{
    using iterator = T*;
    using value_type = T;

    struct metadata_t
    {
        T* _begin;
        T* _end;
    };

    packed_vector() : _storage(nullptr)
    {
    }

    ~packed_vector()
    {
        if (_storage)
            operator delete(_storage);
    }

    packed_vector(std::initializer_list<T>&& values) : packed_vector()
    {
        reallocate(values.size());
        std::copy(std::begin(values), std::end(values), begin());
    }

    template<class It>
    packed_vector(It&& begin, It&& end) : packed_vector()
    {
        std::copy(begin, end, std::back_inserter(*this));
    }

    void push_back(T value)
    {
        reallocate(size() + 1);
        *std::prev(end()) = value;
    }

    std::size_t size()
    {
        return std::distance(begin(), end());
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

    metadata_t* allocate_new(std::size_t size)
    {
        void* raw = ::operator new(size * sizeof(T) + sizeof(metadata_t));
        metadata_t* storage = new (raw) metadata_t{};
        storage->_begin = new (next<metadata_t>(storage)) T[size];
        storage->_end = std::next(storage->_begin, size);
        return storage;
    }

    void reallocate(std::size_t new_size)
    {
        auto* new_storage = allocate_new(new_size);
        if (_storage)
            std::copy(_storage->_begin, _storage->_end, new_storage->_begin);
        _storage = new_storage;
    }

    metadata_t* _storage;
};

// the main reason why you might be interested in this class
static_assert(sizeof(packed_vector<int>) <= sizeof(void*));
