#include "../src/Optional/Optional.hpp"
#include "Tester.hpp"

bool empty_test()
{
    Optional<int> o{};
    
    return o.empty();
}

bool set_function_test()
{
    Optional<int> o{};
    o.set(7);
    
    return o.get() == 7;
}

bool variadic_constructor_test()
{
    Optional<std::pair<size_t, size_t>> o{};
    o.set(1, 2);
    
    return o.get().first == 1 && o.get().second == 2;
}

bool l_value_constructor_test()
{
    Tester t{};
    Optional<Tester> o{t};
    
    return o.get()._member_id - 1 == t._member_id && o.get()._member_value == t._member_value;
}

bool r_value_constructor_test()
{
    Tester t{};
    Optional<Tester> o{std::move(t)};
    
    return o.get()._member_id == t._member_id && o.get()._member_value == t._member_value;
}

bool copy_constructor_test()
{
    Optional<Tester> o1{Tester{}};
    Optional<Tester> o2{o1};
    
    return o1.get()._member_id == o2.get()._member_id - 1 && o1.get()._member_value == o2.get()._member_value;
}

bool move_constructor_test()
{
    Optional<Tester> o1{Tester{}};

    size_t id = o1.get()._member_id;
    size_t value = o1.get()._member_value;

    Optional<Tester> o2{std::move(o1)};
    
    return o2.get()._member_id == id && o2.get()._member_value == value;
}

bool copy_operator_test()
{
    Optional<Tester> o1{Tester{}};
    Optional<Tester> o2 = o1;
    
    return o1.get()._member_id == o2.get()._member_id - 1 && o1.get()._member_value == o2.get()._member_value;
}

bool move_operator_test()
{
    Optional<Tester> o1{Tester{}};

    size_t id = o1.get()._member_id;
    size_t value = o1.get()._member_value;

    Optional<Tester> o2 = std::move(o1);
    
    return o2.get()._member_id == id && o2.get()._member_value == value;
}

bool clear_test()
{
    Optional<int> o{10};
    o.clear();
    
    return o.empty();
}

bool optional_tests()
{
    return empty_test()
        && set_function_test()
        && variadic_constructor_test()
        && l_value_constructor_test()
        && r_value_constructor_test()
        && copy_constructor_test()
        && move_constructor_test()
        && copy_operator_test()
        && move_operator_test();
}