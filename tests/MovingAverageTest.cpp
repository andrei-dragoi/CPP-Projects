#include "../src/MovingAverage/MovingAverage.hpp"

bool empty_test()
{
    MovingAverage<3> ma{};
    
    return ma.get_average() == 0;
};

bool sub_capacity_test()
{
    MovingAverage<3> ma{};
    
    ma.add_number(1);
    ma.add_number(2);
    
    return ma.get_average() == 1.5;
}

bool at_capacity_test()
{
    MovingAverage<3> ma{};
    
    ma.add_number(1);
    ma.add_number(2);
    ma.add_number(3);
    
    return ma.get_average() == 2;
}

bool over_capacity_test()
{
    MovingAverage<3> ma{};
    
    ma.add_number(1);
    ma.add_number(2);
    ma.add_number(3);
    ma.add_number(4);
    
    return ma.get_average() == 3;
}

bool moving_average_tests()
{
    return empty_test()
        && sub_capacity_test()
        && at_capacity_test()
        && over_capacity_test();
}