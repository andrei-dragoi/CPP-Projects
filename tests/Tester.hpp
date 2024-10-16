#pragma once

#include <cstddef>
#include <iostream>

struct Tester
{
    inline static size_t id{};
    size_t _member_id{};
    size_t _member_value{};
    
    static size_t get_next_id()
    {
        return id++;
    }
    
    Tester() 
    : _member_id{ get_next_id() }
    , _member_value{ _member_id } 
    {
    }
    
    Tester(Tester& other)
    : _member_id{ get_next_id() }
    , _member_value{ other._member_value } 
    {
    }
    
    Tester(Tester&& other)
    : _member_id{ other._member_id }
    , _member_value{ other._member_value } 
    {
    }
    
    Tester& operator= (Tester& other)
    {
        if (this == &other)
        {
            return *this;
        }
        
        _member_id = get_next_id();
        _member_value = other._member_value;
        
        return *this;
    }
    
    Tester& operator= (Tester&& other)
    {
        if (this == &other)
        {
            return *this;
        }
        
        _member_id = other._member_id;
        _member_value = other._member_value;
        
        return *this;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Tester& t)
    {
        os << "Tester {id:" << t._member_id << ", value:" << t._member_value << ")\n";
        
        return os;
    }
};