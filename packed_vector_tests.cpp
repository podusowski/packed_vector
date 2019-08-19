#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "packed_vector.hpp"

TEST_CASE("some basics on empty vector")
{
    packed_vector<int> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.begin() == v.end());
}

TEST_CASE("some basics on one element")
{
    packed_vector<int> v{1};
    REQUIRE(v.begin() != v.end());
    REQUIRE(*std::begin(v) == 1);
}

TEST_CASE("some basics on few elements")
{
    packed_vector<int> v{1, 2, 3};
    REQUIRE(v.size() == 3);
    auto begin = v.begin();
    REQUIRE(*begin == 1);
    REQUIRE(*std::next(begin) == 2);
    REQUIRE(*std::next(begin, 2) == 3);
    REQUIRE(std::next(begin, 3) == v.end());
}

TEST_CASE("dynamic insert")
{
    packed_vector<int> v;
    REQUIRE(v.begin() == v.end());

    v.push_back(1);
    REQUIRE(v.size() == 1);
    REQUIRE(v.begin() != v.end());
    REQUIRE(*std::begin(v) == 1);

    v.push_back(2);
    REQUIRE(v.size() == 2);
    REQUIRE(*std::begin(v) == 1);
    REQUIRE(*std::next(v.begin()) == 2);
}
