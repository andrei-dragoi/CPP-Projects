#pragma once

#include <atomic>
#include <cstddef>
#include <stdexcept>


template<typename T>
class SharedPtr
{
private:
    struct ControlBlock
    {
        alignas(alignof(T)) std::byte _resource [sizeof(T)]{};
        std::atomic<size_t> _count{1};
        
        ControlBlock()
        {
        }

        template<typename... Args>
        requires (std::is_constructible_v<T, Args...>)
        ControlBlock(Args&&... args)
        {
            new (&_resource) T(std::forward<Args>(args)...);
        }

        ~ControlBlock() {
            reinterpret_cast<T*>(&_resource)->~T();
        }
    };

public:
    SharedPtr() = default;

    template<typename... Args>
    requires (std::is_constructible_v<T, Args...>)
    SharedPtr(Args&&... args)
    : _control_block_pointer{new ControlBlock{std::forward<Args>(args)...}}
    {
    }

    SharedPtr(T* pointer)
    : _t_pointer{pointer}
    , _control_block_pointer{new ControlBlock{}}
    {
    }
    
    SharedPtr(const SharedPtr& other)
    : _control_block_pointer{other._control_block_pointer}
    , _t_pointer{other._t_pointer}
    {
        if (_control_block_pointer)
        {
            _control_block_pointer->_count.fetch_add(1);
        }
    }
    
    SharedPtr(SharedPtr&& other)
    : _control_block_pointer{other._control_block_pointer}
    , _t_pointer{other._t_pointer}
    {
        other._control_block_pointer = nullptr;
        other._t_pointer = nullptr;
    }
    
    SharedPtr& operator=(const SharedPtr& other)
    {
        if (this != &other)
        {
            free_memory();
            
            _control_block_pointer = other._control_block_pointer;
            _t_pointer = other._t_pointer;
                    
            if (_control_block_pointer)
            {
                _control_block_pointer->_count.fetch_add(1);
            }
        }
        
        return *this;
    }
    
    SharedPtr& operator=(SharedPtr&& other)
    {
        if (this != &other)
        {
            free_memory();
            
            _control_block_pointer = other._control_block_pointer;
            _t_pointer = other._t_pointer;
                    
            other._control_block_pointer = nullptr;
            other._t_pointer = nullptr;
        }
        
        return *this;
    }
    
    T& operator*() const
    {
        if (_t_pointer)
        {
            return *_t_pointer;
        }
        
        if (_control_block_pointer)
        {
            return *reinterpret_cast<T*>(&_control_block_pointer->_resource);
        }
        
        throw std::runtime_error{"SharedPtr object is empty"}; 
    }
    
    T* operator->() const
    {
        return get();
    }
    
    explicit operator bool() const
    {
        return _t_pointer || _control_block_pointer;
    }
    
    T* get() const
    {
        if (_control_block_pointer)
        {
            return reinterpret_cast<T*>(&_control_block_pointer->_resource);
        }
        
        return _t_pointer;
    }
    
    size_t use_count() const 
    {
        return _control_block_pointer ? _control_block_pointer->_count.load() : 0;
    }
    
    ~SharedPtr()
    {
        free_memory();
    }

private:
    ControlBlock* _control_block_pointer{};
    T* _t_pointer{};
    
    void free_memory()
    {
        if (_t_pointer)
        {
            _control_block_pointer->_count.fetch_sub(1);
            
            if (_control_block_pointer->_count == 0)
            {
                delete _control_block_pointer;
                delete _t_pointer;
                
                _control_block_pointer = nullptr;
                _t_pointer = nullptr;
            }
        }
        
        else if (_control_block_pointer)
        {
            _control_block_pointer->_count.fetch_sub(1);
            
            if (_control_block_pointer->_count == 0)
            {
                delete _control_block_pointer;
                
                _control_block_pointer = nullptr;
                _t_pointer = nullptr;
            }
        }
    }
};