#pragma once

#include <new>
#include <bit>
#include <atomic>
#include <array>
#include <type_traits> 

template<typename T, size_t size = 128>
requires (std::is_move_assignable_v<T>)
class RingBuffer
{
using buffer_entry = std::pair<std::atomic<bool>, T>;

public:
    bool read(T& t)
    {
        size_t index = get_index(_readIndex);
        
        std::atomic<bool>& bool_ref = _buffer[index].first;
        bool cant_read = !bool_ref.load(std::memory_order_relaxed);
        
        if (cant_read)
        {
            return false;
        }
        
        T& t_ref = _buffer[index].second;
        
        t = std::move(t_ref);
        t_ref.~T();
        
        bool_ref.store(false, std::memory_order_release);
        
        ++_readIndex;
        
        return true;
    }
    
    bool write(T& t)
    {
        size_t index = get_index(_writeIndex);
        
        std::atomic<bool>& bool_ref = _buffer[index].first;
        bool cant_write = bool_ref.load(std::memory_order_relaxed);
        
        if (cant_write)
        {
            return false;
        }
        
        _buffer[index].second = std::move(t);
        
        bool_ref.store(true, std::memory_order_release);
        
        ++_writeIndex;
        
        return true;
    }

private:
    alignas(std::hardware_destructive_interference_size) std::array<buffer_entry, std::bit_ceil(size)> _buffer{};
    alignas(std::hardware_destructive_interference_size) size_t _readIndex{};
    alignas(std::hardware_destructive_interference_size) size_t _writeIndex{};
    
    size_t get_index(size_t index)
    {
        return (index & (_buffer.size() - 1));
    }
};