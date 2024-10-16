#include "../src/sqrt_func/sqrt_func.hpp"

#include <cmath>
#include <limits>

bool negative_test()
{
    return sqrt_func(-9) == -1;
}

bool zero_test()
{
    return std::fabs(sqrt_func(0) - 0) <= 0.00001;
}

bool one_test()
{
    return std::fabs(sqrt_func(1) - 1) <= 0.00001;
}

bool four_test()
{
    return std::fabs(sqrt_func(4) - 2) <= 0.00001;
}

bool nine_test()
{
    return std::fabs(sqrt_func(9) - 3) <= 0.00001;
}

bool one_hundred_fourty_four_test()
{
    return std::fabs(sqrt_func(144) - 12) <= 0.00001;
}

bool sqrt_func_ten_one_decimal()
{
    return std::fabs(sqrt_func(10, 0.1) - 3.1) <= 0.1;
}

bool sqrt_func_ten_two_decimal()
{
    return std::fabs(sqrt_func(10, 0.01) - 3.16) <= 0.01;
}

bool sqrt_func_ten_three_decimal()
{
    return std::fabs(sqrt_func(10, 0.001) - 3.162) <= 0.001;
}

bool sqrt_func_ten_four_decimal()
{
    return std::fabs(sqrt_func(10, 0.0001) - 3.1622) <= 0.0001;
}

bool sqrt_func_tests()
{
    return negative_test()
        && zero_test()
        && one_test()
        && four_test()
        && nine_test()
        && one_hundred_fourty_four_test()
        && sqrt_func_ten_one_decimal()
        && sqrt_func_ten_two_decimal()
        && sqrt_func_ten_three_decimal()
        && sqrt_func_ten_four_decimal();
}