#include "sqrt_func.hpp"

#include <cmath>
#include <cstdlib>

double sqrt_func(double input, double epsilon = 0.00001)
{
    if (input < 0)
    {
        return -1;
    }
    
    if (input == 0 || input == 1)
    {
        return input;
    }
    
    double lower = 0;
    double upper = input;
    double mid = 0;
    double midSquared = 0;
    
    while(std::abs(input - (mid * mid)) > epsilon)
    {
        mid = (upper + lower) / 2;
        midSquared = mid * mid;
        
        if (midSquared == input)
        {
            break;
        }
        
        else if (mid * mid > input)
        {
            upper = mid;
        }
        
        else
        {
            lower = mid;
        }
    }
    
    return mid;
}