#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "packed_vector.hpp"

TEST_CASE("some basics on empty vector")
{
    packed_vector<int> v;
    REQUIRE(v.begin() == v.end());
}

TEST_CASE("some basics on one element")
{
    packed_vector<int> v{1};
    REQUIRE(v.begin() != v.end());
    REQUIRE(*std::begin(v) == 1);
}
