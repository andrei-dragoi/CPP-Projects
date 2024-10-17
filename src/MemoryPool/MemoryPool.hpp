#pragma once

#include <vector>
#include <memory>
#include <cstddef>
#include <type_traits>

template<typename T>
class MemoryPool
{
private:
    struct Entry
    {
        alignas(alignof(T)) std::byte _buffer[sizeof(T)];  
    };
    
public:
    MemoryPool() = default;

    explicit MemoryPool(size_t pool_size)
    : _allocation_list{}
    , _memory_pointer_stack{}
    , _total_size{}
    {
        reserve_impl(pool_size);
    }
    
    MemoryPool(const MemoryPool& other) = delete;
    
    MemoryPool(MemoryPool&& other) 
    : _allocation_list{std::move(other._allocation_list)}
    , _memory_pointer_stack{std::move(other._memory_pointer_stack)}
    , _total_size{other._total_size}
    {
        other._total_size = 0;
    }
    
    MemoryPool& operator=(const MemoryPool& other) = delete;
    
    MemoryPool& operator=(MemoryPool&& other)
    {
        if (this != &other)
        {
            _allocation_list = std::move(other._allocation_list);
            _memory_pointer_stack = std::move(other._memory_pointer_stack);
            _total_size = other._total_size;

            other._total_size = 0;
        }

        return *this;
    }

    void reserve(size_t reserve_size)
    {
        reserve_impl(reserve_size);
    }
    
    template<typename... Args>
    requires (std::is_constructible_v<T, Args...>)
    T& get_object(Args&&... args)
    {
        T* allocate_pointer = get_memory();
        
        new (allocate_pointer) T(std::forward<Args>(args)...);
        
        return *allocate_pointer;
    }
    
    void return_object(T& return_object)
    {
        return_memory(&return_object);
    }

    size_t objects_used() const
    {
        return _total_size - _memory_pointer_stack.size();
    }

    size_t objects_left() const
    {
        return _memory_pointer_stack.size();
    }

private:
    std::vector<std::pair<std::unique_ptr<Entry[]>, size_t>> _allocation_list{};
    std::vector<Entry*> _memory_pointer_stack{};
    size_t _total_size{};

    void reserve_impl(size_t reserve_size)
    {
        reserve_size = (reserve_size == 0) ? 1 : reserve_size;
        
        _allocation_list.emplace_back(std::make_unique<Entry[]>(reserve_size), reserve_size);

        std::unique_ptr<Entry[]>& allocated_array = _allocation_list.back().first; 

        for (size_t i = 0; i < reserve_size; ++i)
        {
            _memory_pointer_stack.push_back(allocated_array.get() + i); 
        }

        _total_size += reserve_size;
    }

    T* get_memory()
    {
        if (_memory_pointer_stack.empty())
        {
            reserve_impl(_total_size);
        }

        Entry* allocate_pointer = _memory_pointer_stack.back();
        _memory_pointer_stack.pop_back();

        return reinterpret_cast<T*>(allocate_pointer);
    }

    void return_memory(T* deallocate_pointer)
    {
        Entry* deallocate_pointer_as_entry = reinterpret_cast<Entry*>(deallocate_pointer);
        bool valid_address{};

        for (auto& allocation : _allocation_list)
        {
            Entry* allocated_array = allocation.first.get();
            size_t allocated_size = allocation.second;

            if (deallocate_pointer_as_entry >= allocated_array && deallocate_pointer_as_entry <= allocated_array + allocated_size)
            {
                valid_address = true;

                break;
            }
        }
        
        deallocate_pointer->~T();

        if (valid_address)
        {
            _memory_pointer_stack.push_back(reinterpret_cast<Entry*>(deallocate_pointer));
        }
    }
};