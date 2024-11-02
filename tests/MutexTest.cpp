#include "../src/Mutex/Mutex.hpp"

#include <thread>

void count(Mutex& lock, int& counter)
{
    for (size_t i = 0; i < 500; ++i)
    {
        LockGuard lg{lock};
    
        ++counter;
    }
}

bool lock_mt_test()
{
    Mutex lock{};
    
    for (size_t i = 0; i < 100; ++i)
    {
        int counter{};
        
        {
            std::jthread j1{[&]() { count(lock, counter); }};
            std::jthread j2{[&]() { count(lock, counter); }};
        }
        
        if (counter != 1000)
        {
            return false;
        }
    }
    
    return true;
}

bool mutex_test()
{
    return lock_mt_test();
}