#pragma once

#include <vector>

#include "Order.hpp"

struct Transaction
{
    OrderId _order_id{};
    Symbol _symbol{};
    Price _price{};
    Quantity _quantity{};

    bool operator==(const Transaction& other) const
    {
        return _order_id == other._order_id
            && _symbol == other._symbol
            && _price == other._price
            && _quantity == other._quantity;
    }

    bool operator!=(const Transaction& other) const
    {
        return !(*this == other);
    }
    
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

    bool operator==(const TradeRecorder& other) const
    {
        if (_trades.size() != other._trades.size())
        {
            return false;
        }

        for (size_t i = 0; i < _trades.size(); ++i)
        {
            if (_trades[i] != other._trades[i])
            {
                return false;
            }
        }

        return true;
    }

    auto& record() const
    {
        return _trades;
    }

    void clear()
    {
        _trades.clear();
    }
};