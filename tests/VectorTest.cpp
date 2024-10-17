#include "../src/Vector/Vector.hpp"

#include <stdexcept>

bool empty_test()
{
    Vector<int> v{};
    
    return v.empty() && v.size() == 0;
}

bool initializer_list_constructor_test()
{
    Vector<size_t> v{0, 1, 2, 3, 4, 5};
    
    for (size_t i = 0; i <= 5; ++i)
    {
        if (v[i] != i)
        {
            return false;
        }
    }
    
    return true;
}

bool size_constructor_test()
{
    Vector<size_t> v(7);
    
    return v.empty() && v.capacity() == 7;
}

bool size_default_value_constructor_test()
{
    Vector<int> v(7, -1);
    
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (v[i] != -1)
        {
            return false;
        }
    }
    
    return v.size() == 7 && v.capacity() == 7;
}

bool copy_contructor_test()
{
    Vector<int> v1(7, -1);
    Vector<int> v2{v1};
    
    for (size_t i = 0; i < v1.size(); ++i)
    {
        if (v1[i] != -1 || v2[i] != -1)
        {
            return false;
        }
    }
    
    return v1.size() == v2.size() && v1.capacity() == v2.capacity();
}

bool move_contructor_test()
{
    Vector<int> v1(7, -1);
    Vector<int> v2{std::move(v1)};
    
    for (size_t i = 0; i < v1.size(); ++i)
    {
        if (v2[i] != -1)
        {
            return false;
        }
    }
    
    return v2.capacity() == 7 && v1.capacity() == 0;
}

bool copy_operator_test()
{
    Vector<int> v1(7, -1);
    Vector<int> v2 = v1;;
    
    for (size_t i = 0; i < v1.size(); ++i)
    {
        if (v1[i] != -1 || v2[i] != -1)
        {
            return false;
        }
    }
    
    return v1.size() == v2.size() && v1.capacity() == v2.capacity();
}

bool move_operator_test()
{
    Vector<int> v1(7, -1);
    Vector<int> v2 = std::move(v1);
    
    for (size_t i = 0; i < v1.size(); ++i)
    {
        if (v2[i] != -1)
        {
            return false;
        }
    }
    
    return v2.capacity() == 7 && v1.capacity() == 0;
}

bool front_test()
{
    Vector<size_t> v1{1, 2, 3, 4, 5};
    
    return v1.front() == 1;
}

bool back_test()
{
    Vector<size_t> v1{1, 2, 3, 4, 5};
    
    return v1.back() == 5;
}

bool push_back_copy_test()
{
    Vector<size_t> v1{};
    v1.push_back(10);
    
    return v1.back() == 10;
}

bool push_back_variadic_test()
{
    Vector<std::pair<int, int>> v1{};
    v1.push_back(10, 11);
    
    return v1.back().first == 10 && v1.back().second == 11;
}

bool begin_test()
{
    Vector<size_t> v{1, 2, 3, 4, 5};
    
    return *v.begin() == 1;
}

bool iterator_test()
{
    Vector<size_t> v{1, 2, 3, 4, 5};
    
    size_t size{};
    for (auto i = v.begin(); i != v.end(); ++i)
    {
        ++size;
    }
    
    return size == v.size();
}

bool insert_empty()
{
    Vector<size_t> v{};
    v.insert(v.begin(), 1);
    
    return v.size() == 1 && v.front() == 1;
}

bool insert_empty_catch_error()
{
    Vector<size_t> v{};
    
    try
    {
        auto it = v.begin() + 1;
        v.insert(it, 5);
    }
    
    catch (std::runtime_error& e)
    {
        return true;
    }
    
    return false;
}

bool insert_start_test()
{
    Vector<size_t> v{1, 2, 3, 4};
    
    auto it = v.begin();
    
    v.insert(it, 0);
    
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (v[i] != i)
        {
            return false;
        }
    }
    
    return true;
}

bool insert_middle_test()
{
    Vector<size_t> v{0, 1, 3, 4};
    
    auto it = v.begin();
    it += 2;
    
    v.insert(it, 2);
    
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (v[i] != i)
        {
            return false;
        }
    }
    
    return true;
}

bool insert_end_test()
{
    Vector<size_t> v{0, 1, 2, 3};
    
    auto it = v.end();
    
    v.insert(it, 4);
    
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (v[i] != i)
        {
            return false;
        }
    }
    
    return true;
}

bool erase_empty()
{
    Vector<size_t> v{};
    
    try
    {
        v.erase(v.begin());
    }
    
    catch (std::runtime_error& e)
    {
        return true;
    }
    
    return false;
}

bool erase_start()
{
    Vector<size_t> v{4, 0, 1, 2, 3};
    
    auto it = v.begin();
    
    v.erase(it);
    
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (v[i] != i)
        {
            return false;
        }
    }
    
    return true;
}

bool erase_middle()
{
    Vector<size_t> v{0, 1, 5, 2, 3, 4};
    
    auto it = v.begin();
    it += 2;
    
    v.erase(it);
    
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (v[i] != i)
        {
            return false;
        }
    }
    
    return true;
}

bool erase_end()
{
    Vector<size_t> v{0, 1, 2, 3, 4, 10};
    
    auto it = v.end();
    --it;
    
    v.erase(it);
    
    for (size_t i = 0; i < v.size(); ++i)
    {
        if (v[i] != i)
        {
            return false;
        }
    }
    
    return true;
}

bool const_test()
{
    const Vector<size_t> v{1, 2, 3};

    return v.front() == 1 && v.back() == 3 && v.end() - v.begin() == 3;
}

bool vector_tests()
{
    return empty_test()
        && initializer_list_constructor_test()
        && size_constructor_test()
        && size_default_value_constructor_test()
        && copy_contructor_test()
        && move_contructor_test()
        && copy_operator_test()
        && move_operator_test()
        && front_test()
        && back_test()
        && push_back_copy_test()
        && push_back_variadic_test()
        && begin_test()
        && iterator_test()
        && insert_empty()
        && insert_empty_catch_error()
        && insert_start_test()
        && insert_middle_test()
        && insert_end_test()
        && erase_empty()
        && erase_start()
        && erase_middle()
        && erase_end()
        && const_test();
}