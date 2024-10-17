#pragma once

#include <type_traits>
#include <stdexcept>

//without make_unique() functionality
template<typename T>
class UniquePtr
{
public:
    UniquePtr() = default;
    
    template<typename... Args>
    requires (std::is_constructible_v<T, Args...>)
    UniquePtr(Args&&... args)
    : _pointer{new T(std::forward<Args>(args)...)}
    {
    }
    
    UniquePtr(T* pointer)
    : _pointer{pointer}
    {
    }
    
    UniquePtr(const UniquePtr& other) = delete;
    
    UniquePtr(UniquePtr&& other)
    : _pointer{other._pointer}
    {
        other._pointer = nullptr;
    }
    
    UniquePtr& operator=(const UniquePtr& other) = delete;
    
    UniquePtr& operator=(UniquePtr&& other)
    {
        if (this == &other)
        {
            return *this;
        }
        
        delete _pointer;
        
        _pointer = other._pointer;
        other._pointer = nullptr;
        
        return *this;
    }
    
    T& operator*() const
    {
        if (!_pointer)
        {
            throw std::runtime_error{"SharedPtr object is empty"}; 
        }

        return *_pointer;
    }
    
    T* operator->() const
    {
        return _pointer;
    }
    
    T* get() const
    {
        return _pointer;
    }

    template<typename... Args>
    requires (std::is_constructible_v<T, Args...>)
    void set(Args&&... args)
    {
        if (_pointer)
        {
            _pointer->~T();
        }

        new (_pointer) T(std::forward<Args>(args)...);
    }
    
    T* release()
    {
        T* return_pointer = _pointer;
        
        _pointer = nullptr;
        
        return return_pointer;
    }
    
    explicit operator bool() const
    {
        return _pointer;
    }
    
    ~UniquePtr()
    {
        delete _pointer;
    }

private:
    T* _pointer{};
};