#pragma once

#include <list>
#include <unordered_map>

#include "Order.hpp"
#include "TradeRecorder.hpp"

using BookIter = std::list<std::list<Order>>*;
using LevelIter = std::list<std::list<Order>>::iterator;
using OrderIter = std::list<Order>::iterator;

struct RemoveOrderInfo
{
    BookIter _book_iter{};
    LevelIter _level_iter{};
    OrderIter _order_iter{};
};

class Book
{
public:
    void add(Order& order, TradeRecorder& trade_recorder)
    {
        if (_remove_order_map.count(order._order_id))
        {
            return;
        }
        
        if (order._side == Side::bid)
        {
            add_impl(order, _buy_orders, _sell_orders, _buy_cross_comp, _buy_level_comp, trade_recorder);
        }
        
        else
        {
            add_impl(order, _sell_orders, _buy_orders, _sell_cross_comp, _sell_level_comp, trade_recorder);
        }
    }
    
    void remove(OrderId order_id)
    {
        auto remove_order_map_iter = _remove_order_map.find(order_id);
        
        if (remove_order_map_iter != _remove_order_map.end())
        {
            auto book_iter = remove_order_map_iter->second._book_iter;
            auto level_iter = remove_order_map_iter->second._level_iter;
            auto order_iter = remove_order_map_iter->second._order_iter;
            
            level_iter->erase(order_iter);

            if (level_iter->empty())
            {
                book_iter->erase(level_iter);
            }
        }
    }

private:
    using OrderBook = std::list<std::list<Order>>;
    using RemoveOrderMap = std::unordered_map<OrderId, RemoveOrderInfo>;

    OrderBook _buy_orders{};
    OrderBook _sell_orders{};
    RemoveOrderMap _remove_order_map{};
    
    static constexpr auto _buy_cross_comp{ [] (Order& buy_order, Order& sell_order) { return buy_order._price >= sell_order._price; } };
    static constexpr auto _sell_cross_comp{ [] (Order& sell_order, Order& buy_order) { return sell_order._price <= buy_order._price; } };

    static constexpr auto _buy_level_comp{ [] (Order& this_order, Order& other_order) { return this_order._price < other_order._price; } };
    static constexpr auto _sell_level_comp{ [] (Order& this_order, Order& other_order) { return this_order._price > other_order._price; } };

    void add_impl(Order& order, auto& this_side_order_book, auto& other_side_order_book, auto cross_comp, auto level_comp, TradeRecorder& trade_recorder)
    {
        auto other_side_level_iter = other_side_order_book.begin();

        while (!other_side_order_book.empty() && cross_comp(order, *other_side_level_iter->begin()) && order._quantity > 0)
        {
            auto& other_side_order = *other_side_level_iter->begin();
            
            auto fill_symbol = order._symbol;
            auto fill_quantity = std::min(order._quantity, other_side_order._quantity);
            auto fill_price = other_side_order._price;
            
            trade_recorder.add_transaction(Transaction{other_side_order._order_id, fill_symbol, fill_price, fill_quantity});
            trade_recorder.add_transaction(Transaction{order._order_id, fill_symbol, fill_price, fill_quantity});

            order._quantity -= fill_quantity;
            other_side_order._quantity -= fill_quantity;

            if (other_side_order._quantity == 0)
            {
                remove(other_side_order._order_id);
            }

            other_side_level_iter = other_side_order_book.begin();
        }

        if (order._quantity > 0)
        {
            auto this_side_level_iter = this_side_order_book.begin();
            auto this_side_level_end = this_side_order_book.end();
            
            while (this_side_level_iter != this_side_level_end && level_comp(order, *this_side_level_iter->begin()))
            {
                ++this_side_level_iter;
            }

            if (this_side_level_iter == this_side_level_end || order._price != this_side_level_iter->begin()->_price)
            {
                this_side_level_iter = this_side_order_book.insert(this_side_level_iter, std::list<Order>{});
            }

            this_side_level_iter->push_back(order);

            _remove_order_map[order._order_id] = {&this_side_order_book, this_side_level_iter, std::prev(this_side_level_iter->end())};
        }
    }
};