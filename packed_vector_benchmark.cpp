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

int main(int argc, const char* argv[])
{
    plyometrics::run_all(argc, argv);
}
