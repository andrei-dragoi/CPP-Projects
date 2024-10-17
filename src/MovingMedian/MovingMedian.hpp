#pragma once

#include <queue>
#include <cstddef>

template<typename T>
requires (std::is_integral_v<T> || std::is_floating_point_v<T>)
class MovingMedian
{
public:
    MovingMedian() = default;
    
    void add_number(T num)
    {
        _maxHeap.push(num);
        
        if (!_minHeap.empty() && _maxHeap.top() > _minHeap.top())
        {
            T temp = _maxHeap.top();
            _maxHeap.pop();
            
            _maxHeap.push(_minHeap.top());
            _minHeap.pop();
            _minHeap.push(temp);
        }

        if (_maxHeap.size() - _minHeap.size() > 1)
        {    
            _minHeap.push(_maxHeap.top());
            _maxHeap.pop();
        }
    }
    
    T get_average() const
    {
        T max_of_smaller_elements = (_maxHeap.empty()) ? 0 : _maxHeap.top();
        T min_of_bigger_elements = (_minHeap.empty()) ? 0 : _minHeap.top();
        
        size_t element_count = (!_maxHeap.empty()) + (!_minHeap.empty()) + (_maxHeap.empty() && _minHeap.empty());

        return ((_minHeap.size() + _maxHeap.size()) & 1) ? 
            max_of_smaller_elements : (max_of_smaller_elements + min_of_bigger_elements) / element_count;
    }

private:
    std::priority_queue<T, std::vector<T>, std::less<T>> _maxHeap{};
    std::priority_queue<T, std::vector<T>, std::greater<T>> _minHeap{};
};