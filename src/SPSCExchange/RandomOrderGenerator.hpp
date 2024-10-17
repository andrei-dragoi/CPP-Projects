#pragma once

#include "../Exchange/Order.hpp"

struct RandomOrderGenerator
{
    std::uint64_t _max_symbol{};
    std::uint64_t _max_price{};
    std::uint64_t _max_quantity{};
    std::uint64_t _order_id{};
    static constexpr Side _enum_list [] = {Side::bid, Side::ask};

    RandomOrderGenerator(std::uint64_t max_symbol, std::uint64_t max_price, std::uint64_t max_quantity)
    : _max_symbol{max_symbol}, _max_price{max_price}, _max_quantity{max_quantity}
    {
    }

    Order generate()
    {
        ++_order_id;
        std::uint64_t random_symbol = 1 + std::rand() % (_max_symbol); 
        std::uint64_t random_price = 1 + std::rand() % (_max_price); 
        std::uint64_t random_quantity = 1 + std::rand() % (_max_quantity); 
        Side random_side = _enum_list[std::rand() & 1]; 

        return Order{_order_id, random_symbol, random_price, random_quantity, random_side};
    }
};