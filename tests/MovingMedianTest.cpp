#include "../src/MovingMedian/MovingMedian.hpp"

bool empty_test()
{
    MovingMedian ma{};
    
    return ma.get_average() == 0;
}

bool one_element_test()
{
    MovingMedian ma{};
    
    ma.add_number(10);
    
    return ma.get_average() == 10;
}

bool even_element_count_test()
{
    MovingMedian ma{};
    
    ma.add_number(10);
    ma.add_number(20);
    ma.add_number(30);
    ma.add_number(40);
    
    return ma.get_average() == 25;
}

bool odd_element_count_test()
{
    MovingMedian ma{};
    
    ma.add_number(10);
    ma.add_number(20);
    ma.add_number(30);
    
    return ma.get_average() == 20;
}

bool random_order_test()
{
    MovingMedian ma{};
    
    ma.add_number(20);
    ma.add_number(60);
    ma.add_number(70);
    ma.add_number(40);
    ma.add_number(50);
    ma.add_number(30);
    ma.add_number(80);
    ma.add_number(30);
    ma.add_number(10);
    
    return ma.get_average() == 50;
}

bool moving_median_tests()
{
    return empty_test()
        && one_element_test()
        && even_element_count_test()
        && odd_element_count_test()
        && random_order_test();
}