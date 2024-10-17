#pragma once

#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <iostream>
#include <vector>

enum class Currency
{
    USD, 
    CAD, 
    MEX, 
    GBP, 
    RON, 
    JPY,
    //...
};

inline std::ostream& operator<<(std::ostream& os, const Currency& currency)
{
    switch(currency)
    {
        case Currency::USD:
            os << "USD";
            break;
        case Currency::CAD:
            os << "CAD";
            break;
        case Currency::MEX:
            os << "MEX";
            break;
        case Currency::GBP:
            os << "GBP";
            break;
        case Currency::RON:
            os << "RON";
            break;
        case Currency::JPY:
            os << "JPY";
            break;
    }
    
    return os;
}

class ForeignExchange
{
private:
    struct CurrencyInfo
    {
        std::unordered_map<Currency, double> _rates{};
        Currency _currency{};

        CurrencyInfo(Currency currency) : _currency{currency}
        {
        }
        
        CurrencyInfo() = default;
    };

public:
    ForeignExchange() = default;

    void add(Currency from_currency, Currency to_currency, double fx_rate)
    {
        _currency_graph[from_currency]._rates.emplace(to_currency, fx_rate);
    }

    void remove(Currency to_remove)
    {
        for (auto& [currency, currency_info] : _currency_graph)
        {
            auto& currency_graph = currency_info._rates;
            auto to_remove_iter = currency_graph.find(to_remove);

            if (to_remove_iter != currency_graph.end())
            {
                currency_graph.erase(to_remove_iter);
            }
        }

        _currency_graph.erase(to_remove);
    }

    std::vector<double> get_all_fx_rates(Currency from_currency, Currency to_currency)
    {
        std::vector<double> final_fx_rates{};
        std::unordered_set<Currency> visited{from_currency};
        std::deque<std::pair<Currency, double>> q{{from_currency, 1}};

        while(!q.empty())
        {
            auto& current_from_currency = q.front().first;
            auto current_rate = q.front().second;

            for (const auto& [current_to_currency, current_to_rate] : _currency_graph[current_from_currency]._rates)
            {
                if (current_to_currency == to_currency)
                {
                    final_fx_rates.push_back(current_rate * current_to_rate);
                }

                if (!visited.count(current_to_currency))
                {
                    q.emplace_back(current_to_currency, current_rate * current_to_rate);
                    visited.insert(current_to_currency);
                }
            }

            q.pop_front();
        }

        return final_fx_rates;
    }

    void print()
    {
        for (auto& [currency, currency_info] : _currency_graph)
        {
            auto& currency_graph = currency_info._rates;
            
            std::cout << "From " << currency << ":\n";

            for (auto& [to_currency, rate] : currency_graph)
            {
                std::cout << "To " << to_currency << ' ' << rate << '\n';
            }
            std::cout << '\n';
        }
    } 

private:
    std::unordered_map<Currency, ForeignExchange::CurrencyInfo> _currency_graph{};
};