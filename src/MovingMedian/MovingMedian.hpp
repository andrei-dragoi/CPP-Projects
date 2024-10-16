#pragma once

#include <queue>
#include <cstddef>

class MovingMedian
{
public:
    MovingMedian() = default;
    
    void add_number(double num)
    {
        _maxHeap.push(num);
        
        if (!_minHeap.empty() && _maxHeap.top() > _minHeap.top())
        {
            double temp = _maxHeap.top();
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
    
    double get_average() const
    {
        double max_of_smaller_elements = (_maxHeap.empty()) ? 0 : _maxHeap.top();
        double min_of_bigger_elements = (_minHeap.empty()) ? 0 : _minHeap.top();
        
        size_t element_count = (!_maxHeap.empty()) + (!_minHeap.empty()) + (_maxHeap.empty() && _minHeap.empty());

        return ((_minHeap.size() + _maxHeap.size()) & 1) ? 
            max_of_smaller_elements : (max_of_smaller_elements + min_of_bigger_elements) / element_count;
    }

private:
    std::priority_queue<double, std::vector<double>, std::less<double>> _maxHeap{};
    std::priority_queue<double, std::vector<double>, std::greater<double>> _minHeap{};
};