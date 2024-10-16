#pragma once

#include <cstddef>

template<size_t size>
class MovingAverage
{
public:
    MovingAverage() = default;
    
    void add_number(double num)
    {
        _index = (_index == size) ? 0 : _index;
        _sum -= _buffer[_index];
        
        _buffer[_index] = num;
        _sum += _buffer[_index];
        
        ++_index;
        ++_iterations;
    }
    
    double get_average() const
    {
        double element_count = (_iterations >= size) ? size : _iterations;
        
        return (_iterations == 0) ? 0 : _sum / element_count;
    }

private:
    double _buffer [size]{};
    double _sum{};
    size_t _index{};
    size_t _iterations{};
};