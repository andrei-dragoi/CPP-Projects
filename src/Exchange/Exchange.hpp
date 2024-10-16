#pragma once

#include <list>

#include "Order.hpp"
#include "Book.hpp"

class Exchange
{
public:
    Exchange(TradeRecorder& trade_recorder)
    : _trade_recorder{trade_recorder}
    {
    }

    void add_order(Order order)
    {
        _book_map[order._symbol].add(order, _trade_recorder);
    }

    void remove_order(OrderId order_id, Symbol symbol)
    {
        _book_map[symbol].remove(order_id);
    }

private:
    using BookMap = std::unordered_map<Symbol, Book>;

    BookMap _book_map{};

    TradeRecorder& _trade_recorder;
};