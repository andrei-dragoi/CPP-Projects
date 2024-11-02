#pragma once

#include <atomic>

class Mutex
{
public:
    void lock()
    {
        while (true)
        {
            while(_locked.load(std::memory_order_relaxed))
            {
            }
            
            if (!_locked.exchange(true, std::memory_order_release)) 
            {
                break;
            }
        }
    }
    
    void unlock()
    {
        _locked.store(false, std::memory_order_release);
    }
    
    bool try_lock()
    {
        return !_locked.exchange(true, std::memory_order_acquire);
    }

private:
    std::atomic<bool> _locked{};
};

class LockGuard
{
public:
    LockGuard (Mutex& lock) 
    : _lock{lock}
    {
        _lock.lock();
    }
    
    ~LockGuard()
    {
        _lock.unlock();
    }

private:
    Mutex& _lock;
};