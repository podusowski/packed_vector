#include <array>
#include <memory>

#include "common.hpp"

struct fancy_abstract_class
{
    virtual void foo() = 0;
    virtual ~fancy_abstract_class() = default;
};

template<std::size_t N>
struct fancy_class final : public fancy_abstract_class
{
    __attribute__((noinline)) void foo() override
    {
        asm volatile("");
    }
};

template<std::size_t... Idx>
__attribute__((noinline)) auto fancy_factory_pattern(std::index_sequence<Idx...>)
{
    std::array<std::unique_ptr<fancy_abstract_class>, sizeof...(Idx)> objects{std::make_unique<fancy_class<Idx>>()...};
    return objects;
}

template<class T>
struct type_traits
{
    static constexpr std::size_t s_tag = T::s_tag;
    using type = T;
};

template<class Likely>
__attribute__((noinline)) void call(fancy_abstract_class& object)
{
    if (__builtin_expect(&typeid(object) == &typeid(Likely), true))
        static_cast<Likely&>(object).foo();
    else
        object.foo();
}

template<class Objects, std::size_t... Idx>
__attribute__((noinline)) void work(const Objects& objects, std::index_sequence<Idx...>)
{
    swallow((call<fancy_class<Idx>>(*std::get<Idx>(objects)), int{})...);
}

int main()
{
    auto objects = fancy_factory_pattern(std::make_index_sequence<complexity>());

    for (int i = 0; i < 100000; i++)
        work(objects, std::make_index_sequence<complexity>());
}
