#pragma once

#include <iostream>
#include <cstdint>

enum class Side
{
    bid, 
    ask
};

using OrderId = std::uint64_t;
using Symbol = std::uint64_t;
using Price = std::uint64_t;
using Quantity = std::uint64_t;

struct Order
{
    OrderId _order_id{};
    Symbol _symbol{};
    Price _price{};
    Quantity _quantity{};
    Side _side{};
    
    friend std::ostream& operator<<(std::ostream& os, const Order& order)
    {
        os << "Order(" << order._order_id << ", " << order._symbol << ", " << order._price << ", " << order._quantity << ", " << ((order._side == Side::bid) ? "b)\n" : "a)\n");
        
        return os;
    }   
};