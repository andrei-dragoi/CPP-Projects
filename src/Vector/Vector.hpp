#pragma once

#include <stdexcept>

template<typename T>
class Vector
{
public:
    using Iterator = T*;

    Vector() = default;
    
    Vector(size_t size)
    : _pointer{static_cast<T*>( ::operator new (sizeof(T) * size, std::align_val_t(alignof(T))) )}
    , _capacity{size}
    {
    }
    
    Vector(size_t size, const T& default_t)
    : _pointer{static_cast<T*>( ::operator new (sizeof(T) * size, std::align_val_t(alignof(T))) )}
    , _size{size}
    , _capacity{size}
    {
        for (size_t i = 0; i < size; ++i)
        {
            _pointer[i] = default_t;
        }
    }
    
    Vector(std::initializer_list<T> list)
    : _pointer{static_cast<T*>( ::operator new (sizeof(T) * list.size(), std::align_val_t(alignof(T))) )}
    , _size{list.size()}
    , _capacity{list.size()}
    {
        auto list_iter = list.begin();
        
        for (size_t i = 0; i < _size; ++i)
        {
            _pointer[i] = std::move(*list_iter);
            
            ++list_iter;
        }
    }
    
    Vector(Vector& other)
    : _pointer{static_cast<T*>( ::operator new (sizeof(T) * other._capacity, std::align_val_t(alignof(T))) )}
    , _size{other._size}
    , _capacity{other._capacity}
    {
        for (size_t i = 0; i < _size; ++i)
        {
            _pointer[i] = other._pointer[i];
        }
    }
    
    Vector(Vector&& other)
    : _pointer{other._pointer}
    , _size{other._size}
    , _capacity{other._capacity}
    {
        other._pointer = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    
    Vector& operator= (Vector& other)
    {
        if (this == &other)
        {
            return *this;
        }
        
        destruct_memory();
        free_memory();
        
        _pointer = static_cast<T*>(::operator new (sizeof(T) * other._capacity, std::align_val_t(alignof(T))));
        _size = other._size;
        _capacity = other._capacity;
        
        for (size_t i = 0; i < _size; ++i)
        {
            _pointer[i] = other._pointer[i];
        } 
        
        return *this;
    }
    
    Vector& operator= (Vector&& other)
    {
        if (this == &other)
        {
            return *this;
        }
        
        destruct_memory();
        free_memory();
        
        _pointer = other._pointer;
        _size = other._size;
        _capacity = other._capacity;
        
        return *this;
    }
    
    template<typename... Args>
    requires std::is_constructible_v<T, Args...>
    void push_back(Args&&... args)
    {
        if (_size == _capacity)
        {
            resize_and_copy(_capacity * 2);
        }
        
        new (_pointer + _size) T(std::forward<Args>(args)...);
        
        ++_size;
    }
    
    void pop_back()
    {
        if (_size == 0)
        {
            return;
        }
        
        _pointer[_size].~T();
        --_size;
    }

    T& front()
    {
        if (_size == 0)
        {
            throw std::runtime_error{"Vector is empty"};
        }

        return _pointer[0];
    }

    T& back()
    {
        if (_size == 0)
        {
            throw std::runtime_error{"Vector is empty"};
        }

        return _pointer[_size - 1];
    }

    T& operator[] (size_t index)
    {
        return _pointer[index];
    }
    
    bool empty()
    {
        return _size == 0;
    }

    size_t size()
    {
        return _size;
    }

    size_t capacity()
    {
        return _capacity;
    }

    void resize(size_t extra_capacity)
    {
        resize_and_copy(_capacity + extra_capacity);
    }

    void clear()
    {
        destruct_memory();
        _size = 0;
    }

    Vector::Iterator begin()
    {
        return _pointer;
    }

    Vector::Iterator end()
    {
        return (_pointer == nullptr) ? 
            _pointer : 
                _pointer + _size;
    }

    template<typename... Args>
    requires std::is_constructible_v<T, Args...>    
    void insert(Vector::Iterator iter, Args&&... args)
    {
        if (_capacity == 0)
        {
            if (iter != begin())
            {
                throw std::runtime_error{"Invalid Iterator"};
            }

            push_back(std::forward<Args>(args)...);

            return;
        }

        if (iter < _pointer || iter - _pointer > _capacity)
        {
            throw std::runtime_error{"Invalid Iterator"};
        }

        size_t insert_index = iter - _pointer;

        push_back(std::forward<Args>(args)...);

        for (size_t i = insert_index; i < _size; ++i)
        {
            std::swap(_pointer[i], _pointer[_size - 1]);
        }
    }

    void erase(Vector::Iterator iter)
    {
        if (_capacity == 0 || iter < _pointer || iter - _pointer > _capacity)
        {
            throw std::runtime_error{"Invalid Iterator"};
        }

        size_t erase_index = iter - _pointer;

        for (size_t i = erase_index; i < _size - 1; ++i)
        {
            std::swap(_pointer[i], _pointer[i + 1]);
        }

        pop_back();
    }

private:
    T* _pointer{};
    size_t _size{};
    size_t _capacity{};
    
    void destruct_memory()
    {
        for (size_t i = 0; i < _size; ++i)
        {
            _pointer[i].~T();
        }
    }
    
    void free_memory()
    {
        ::operator delete (_pointer, sizeof(T) * _capacity);
    }
    
    void resize_and_copy(size_t new_capacity)
    {
        new_capacity = (new_capacity== 0) ? 1 : new_capacity;

        T* new_pointer = static_cast<T*>(::operator new (sizeof(T) * new_capacity, std::align_val_t(alignof(T))));
        
        for (size_t i = 0; i < _size; ++i)
        {
            new_pointer[i] = std::move(_pointer[i]);
        }
        
        destruct_memory();
        free_memory();
        
        _pointer = new_pointer;
        _capacity = new_capacity;
    }
    
};