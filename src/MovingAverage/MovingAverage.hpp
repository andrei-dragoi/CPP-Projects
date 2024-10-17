#pragma once

#include <cstddef>
#include <type_traits>

template<typename T, size_t size>
requires (std::is_integral_v<T> || std::is_floating_point_v<T>)
class MovingAverage
{
public:
    MovingAverage() = default;
    
    void add_number(T num)
    {
        _index = (_index == size) ? 0 : _index;
        _sum -= _buffer[_index];
        
        _buffer[_index] = num;
        _sum += _buffer[_index];
        
        ++_index;
        ++_iterations;
    }
    
    T get_average() const
    {
        T element_count = (_iterations >= size) ? size : _iterations;
        
        return (_iterations == 0) ? 0 : _sum / element_count;
    }

private:
    T _buffer [size]{};
    T _sum{};
    size_t _index{};
    size_t _iterations{};
};