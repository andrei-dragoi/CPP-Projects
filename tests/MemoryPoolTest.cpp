#include "../src/MemoryPool/MemoryPool.hpp"
#include "NoDefault.hpp"

#include <stdexcept>

bool empty_initialization_test()
{
    MemoryPool<int> mp{};
    auto& ref = mp.get_object();

    return mp.objects_used() == 1;
}

bool contiguous_allocation_test()
{
    size_t memory_pool_size = 10;
    MemoryPool<int> mp{memory_pool_size};

    auto* ptr = &mp.get_object();

    for (size_t i = 1; i < memory_pool_size; ++i)
    {
        auto* new_ptr = &mp.get_object();

        if (ptr - new_ptr != 1)
        {
            return false;
        }

        ptr = new_ptr;
    }

    return true;
}

bool self_reserve_test()
{
    MemoryPool<int> mp{1};
    mp.get_object();
    mp.get_object();

    return mp.objects_used() == 2;
}

bool deallocation_test()
{
    MemoryPool<int> mp{1};

    auto& ref = mp.get_object();
    auto* ptr = &ref;
    mp.return_object(ref);

    return mp.objects_left() == 1 && &mp.get_object() == ptr;
}

bool object_allocation_test()
{
    MemoryPool<std::pair<int, int>> mp{1};

    auto& ref = mp.get_object(5, 6);

    return ref.first == 5 && ref.second == 6;
}

bool object_deallocation_test()
{
    MemoryPool<std::pair<int, int>> mp{1};

    auto& ref = mp.get_object(5, 6);
    auto* ptr = &ref;
    mp.return_object(ref);

    return &mp.get_object() == ptr;
}

bool resizing_test()
{
    size_t memory_pool_size = 10;
    MemoryPool<int> mp{memory_pool_size};

    for (size_t i = 0; i < memory_pool_size; ++i)
    {
        mp.get_object();
    }
    mp.get_object();

    return mp.objects_used() == 11 && mp.objects_left() == 9;
}

bool no_default_test()
{
    try
    {
        size_t memory_pool_size = 10;
        MemoryPool<int> mp{memory_pool_size};    
    }
    
    catch (std::runtime_error& e)
    {
        return false;
    }
    
    return true;
}

bool memory_pool_tests()
{
    return empty_initialization_test()
        && contiguous_allocation_test()
        && self_reserve_test()
        && deallocation_test()
        && object_allocation_test()
        && object_deallocation_test()
        && resizing_test()
        && no_default_test();
}