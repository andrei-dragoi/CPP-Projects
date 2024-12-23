#pragma once

#include <type_traits>
#include <cstddef>
#include <stdexcept>

template<typename T>
class Optional
{
public:
    Optional() = default;
    
    template<typename... Args>
    requires (std::is_constructible_v<T, Args...>)
    Optional(Args&&... args)
    : _has_value{ true }
    {
        new (_buffer) T(std::forward<Args>(args)...);
    }
    
    Optional(const Optional& other)
    : _has_value{ other._has_value }
    {
        if (!other.empty())
        {
            new (_buffer) T(const_cast<T&>(*reinterpret_cast<const T*>(&other._buffer)));
        }
    }
    
    Optional(Optional&& other)
    : _has_value{ other._has_value }
    {
        if (!other.empty())
        {
            new (_buffer) T(std::move(*reinterpret_cast<T*>(other._buffer)));
            other._has_value = false;
        }
    }
    
    Optional& operator=(const Optional& other)
    {
        if (this == &other)
        {
            return *this;
        }

        clear();
        
        if (!other.empty())
        {   
            new (_buffer) T(const_cast<T&>(reinterpret_cast<const T*>(other._buffer)));
        }
        
        _has_value = other._has_value;
        
        return *this;
    }
    
    Optional& operator=(Optional&& other)
    {
        if (this == &other)
        {
            return *this;
        }

        clear();

        _has_value = other._has_value;
        
        if (!other.empty())
        {   
            new (_buffer) T(std::move(*reinterpret_cast<T*>(_buffer)));

            other._has_value = false;
        }
        
        return *this;
    }
    
    T& get()
    {
        if (empty())
        {
            throw std::runtime_error{"Optional object is empty"};
        }
        
        return *reinterpret_cast<T*>(_buffer);
    }

    const T& get() const
    {
        if (empty())
        {
            throw std::runtime_error{"Optional object is empty"};
        }
        
        return *reinterpret_cast<const T*>(_buffer);
    }
    
    template<typename... Args>
    requires std::is_constructible_v<T, Args...>
    void set(Args&&... args)
    {
        clear();
        
        _has_value = true;
        new (_buffer) T(std::forward<Args>(args)...);
    }

    void clear()
    {
        if (!empty())
        {
            reinterpret_cast<T*>(_buffer)->~T();
            _has_value = false;
        }
    }
    
    bool empty() const
    {
        return !_has_value;
    }
    
    ~Optional()
    {
        clear();
    }

private:
    alignas(alignof(T)) std::byte _buffer [sizeof(T)]{};
    bool _has_value{};
};