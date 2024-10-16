#include <cassert>

#include "../src/SharedPtr/SharedPtr.hpp"

bool empty_test()
{
    SharedPtr<int> sp{};
    
    return sp.use_count() == 0;
}

bool t_pointer_constructor_test()
{
    SharedPtr<int> sp{new int{7}};
    
    return sp.use_count() == 1;
}

bool variadic_constructor_test()
{
    SharedPtr<int> sp{7};
    
    return sp.use_count() == 1;
}

bool copy_constructor_test()
{
    SharedPtr<int> sp1{7};
    SharedPtr<int> sp2{sp1};
    
    SharedPtr<int> sp3{new int{7}};
    SharedPtr<int> sp4{sp3};
    
    return sp1.use_count() == 2 && sp2.use_count() == 2
        && sp3.use_count() == 2 && sp4.use_count() == 2;
}

bool move_constructor_test()
{
    SharedPtr<int> sp1{7};
    SharedPtr<int> sp2{std::move(sp1)};
    
    SharedPtr<int> sp3{new int{7}};
    SharedPtr<int> sp4{std::move(sp3)};
    
    return sp2.use_count() == 1 && sp4.use_count() == 1;
}

bool copy_operator_test()
{
    SharedPtr<int> sp1{7};
    SharedPtr<int> sp2 = sp1;
    
    SharedPtr<int> sp3{new int{7}};
    SharedPtr<int> sp4 = sp3;
    
    return sp1.use_count() == 2 && sp2.use_count() == 2
        && sp3.use_count() == 2 && sp4.use_count() == 2;
}


bool move_operator_test()
{
    SharedPtr<int> sp1{7};
    SharedPtr<int> sp2 = std::move(sp1);
    
    SharedPtr<int> sp3{new int{7}};
    SharedPtr<int> sp4 = std::move(sp3);
    
    return sp2.use_count() == 1 && sp4.use_count() == 1;
}

bool dereference_operator_error_test()
{
    SharedPtr<int> sp{};
    
    try
    {
        *sp;    
    }
    
    catch (std::runtime_error& e)
    {
        return true;
    }
    
    return false;
}

bool dereference_operator_test()
{
    SharedPtr<int> sp1{7};
    SharedPtr<int> sp2{new int{7}};
    
    return *sp1 == 7 && *sp2 == 7;
}

bool empty_get_test()
{
    SharedPtr<int> sp{};
    
    return !sp;
}

bool get_test()
{
    SharedPtr<int> sp1{7};
    SharedPtr<int> sp2{new int{7}};
    
    return sp1 && sp2;
}

bool shared_ptr_tests()
{
    return empty_test()
        && t_pointer_constructor_test()
        && variadic_constructor_test()
        && copy_constructor_test()
        && move_constructor_test()
        && copy_operator_test()
        && move_operator_test()
        && dereference_operator_error_test()
        && dereference_operator_test()
        && empty_get_test()
        && get_test();
}