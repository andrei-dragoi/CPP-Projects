#include "../src/UniquePtr/UniquePtr.hpp"
#include "Tester.hpp"

bool empty_test()
{
    UniquePtr<Tester> u{};
    
    return !u;
}

bool variadic_constructor_test()
{
    UniquePtr<std::pair<int, int>> u{1, 2};
    
    return u->first == 1 && u->second == 2;
}

bool pointer_constructor_test()
{
    int* int_pointer = new int(7);
    UniquePtr<int> u{int_pointer};
    
    return *u == 7;
}

bool get_test()
{
    int* int_pointer = new int(7);
    UniquePtr<int> u{int_pointer};
    
    return u.get() == int_pointer;
}

bool move_constructor_test()
{
    UniquePtr<Tester> u1{Tester{}};
    
    size_t id = u1->_member_id;
    size_t value = u1->_member_value;
    
    UniquePtr<Tester> u2{std::move(u1)};
    
    return u2->_member_id == id && u2->_member_value == value;
}

bool move_operator_test()
{
    UniquePtr<Tester> u1{Tester{}};
    
    size_t id = u1->_member_id;
    size_t value = u1->_member_value;
    
    UniquePtr<Tester> u2 = std::move(u1);
    
    return u2->_member_id == id && u2->_member_value == value;
}

bool set_test()
{
    UniquePtr<std::pair<int, int>> u{1, 2};
    u.set(2, 1);
    
    return u->first == 2 && u->second == 1;
}

bool unique_ptr_tests()
{
    return empty_test()
        && variadic_constructor_test()
        && pointer_constructor_test()
        && get_test()
        && move_constructor_test()
        && move_operator_test()
        && set_test();
}