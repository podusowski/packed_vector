#include <plyometrics/plyometrics.hpp>

#include <vector>
#include "packed_vector.hpp"

using vectors_spec = plyometrics::spec::with_types<std::vector<int>, packed_vector<int>>;

NBENCHMARK_P(constructing_empty, vectors_spec)
{
    while (loop)
    {
        auto v = loop.type();
        plyometrics::use(v.begin());
    }
}

NBENCHMARK_P(constructing_many, vectors_spec)
{
    auto sequence_data = plyometrics::sequence_range(1024);

    while (loop)
    {
        auto v = loop.type(sequence_data.begin(), sequence_data.end());
        plyometrics::use(v.begin());
    }
}

NBENCHMARK_P(iterating_empty, vectors_spec)
{
    auto v = loop.type();

    while (loop)
        for (auto e : v)
            plyometrics::use(&e);
}

NBENCHMARK_P(iterating_many, vectors_spec)
{
    auto sequence_data = plyometrics::sequence_range(1024);
    auto v = loop.type(sequence_data.begin(), sequence_data.end());

    while (loop)
        for (auto e : v)
            plyometrics::use(&e);
}

template<class T>
__attribute__((noinline)) void swallow(T&& v1, T&& v2, T&& v3)
{
    plyometrics::use(&v1);
    plyometrics::use(&v2);
    plyometrics::use(&v3);
}

NBENCHMARK_P(moving_empty, vectors_spec)
{
    while (loop)
    {
        auto v1 = loop.type();
        auto v2 = loop.type();
        auto v3 = loop.type();
        swallow(std::move(v1), std::move(v2), std::move(v3));
    }
}

template<class T>
__attribute__((noinline)) T&& pass_through(T&& v)
{
    return std::move(v);
}

NBENCHMARK_P(moving_full, vectors_spec)
{
    auto sequence_data = plyometrics::sequence_range(1024);
    auto v = loop.type(sequence_data.begin(), sequence_data.end());

    while (loop)
    {
        v = pass_through(std::move(v));
    }
}

int main(int argc, const char* argv[])
{
    plyometrics::run_all(argc, argv);
}
