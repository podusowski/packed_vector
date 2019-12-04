#include <array>
#include <memory>

#include "common.hpp"

struct fancy_abstract_class
{
    std::size_t tag = 0;

    virtual void foo() = 0;
    virtual ~fancy_abstract_class() = default;
};

template<std::size_t N>
struct fancy_class final : public fancy_abstract_class
{
    static constexpr std::size_t s_tag = N;

    fancy_class()
    {
        tag = N;
    }

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

template<class Objects, std::size_t... Idx>
__attribute__((noinline)) void work(const Objects& objects, std::index_sequence<Idx...>)
{
    auto call = [](auto traits, fancy_abstract_class& object)
    {
        if (__builtin_expect(object.tag, traits.s_tag) == traits.s_tag)
            static_cast<typename decltype(traits)::type&>(object).foo();
        else
            object.foo();

        return int{};
    };

    swallow(call(type_traits<fancy_class<Idx>>{}, *std::get<Idx>(objects))...);
}

int main()
{
    auto objects = fancy_factory_pattern(std::make_index_sequence<complexity>());

    for (int i = 0; i < 100000; i++)
        work(objects, std::make_index_sequence<complexity>());
}
