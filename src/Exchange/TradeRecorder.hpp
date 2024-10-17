#pragma once

#include <vector>

#include "Order.hpp"

struct Transaction
{
    OrderId _order_id{};
    Symbol _symbol{};
    Price _price{};
    Quantity _quantity{};

    auto operator<=>(const Transaction&) const = default;
    
    friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction)
    {
        os << "Transaction(" << transaction._order_id << ", " << transaction._symbol << ", " << transaction._price << ", " << transaction._quantity << ")\n";
        
        return os;
    }  
};

struct TradeRecorder
{
    std::vector<Transaction> _trades{};

    void add_transaction(const Transaction& t)
    {
        _trades.push_back(t);
    }

    auto operator<=>(const TradeRecorder&) const = default;

    const std::vector<Transaction>& record() const
    {
        return _trades;
    }

    void clear()
    {
        _trades.clear();
    }
};