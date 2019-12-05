#include <array>
#include <memory>
#include <vector>
#include <algorithm>

#include "common.hpp"

struct fancy_abstract_class
{
    virtual void foo() = 0;
    virtual ~fancy_abstract_class() = default;
};

struct fancy_class final : public fancy_abstract_class
{
    __attribute__((noinline)) void foo() override
    {
        asm volatile("");
    }
};

__attribute__((noinline)) auto fancy_factory_pattern(std::size_t size)
{
    std::vector<std::unique_ptr<fancy_abstract_class>> ret(size);
    std::generate(ret.begin(), ret.end(), [] { return std::make_unique<fancy_class>(); });
    return ret;
}

#define HINTED_CALL(object, likely, call) \
    if (__builtin_expect(&typeid(object) == &typeid(likely), true)) \
        static_cast<likely&>(object).call; \
    else \
        object.call; \

template<std::size_t, class Likely>
__attribute__((noinline)) void call(fancy_abstract_class& object)
{
    HINTED_CALL(object, Likely, foo());
}

template<class Objects, std::size_t... Idx>
__attribute__((noinline)) void work(const Objects& objects, std::index_sequence<Idx...>)
{
    swallow((call<Idx, fancy_class>(*objects[Idx]), int{})...);
}

int main()
{
    auto objects = fancy_factory_pattern(complexity);

    for (int i = 0; i < 100000; i++)
        work(objects, std::make_index_sequence<complexity>());
}
