#include "../src/ForeignExchange/ForeignExchange.hpp"

bool empty_test()
{
    using enum Currency;
    
    ForeignExchange exchange{};
    
    return exchange.get_all_fx_rates(USD, CAD).empty();
}

bool remove_test()
{
    using enum Currency;
    
    ForeignExchange exchange{};
    
    exchange.add(USD, RON, 4.60288);
    
    exchange.remove(USD);
    
    return exchange.get_all_fx_rates(USD, CAD).empty();
}

bool simple_cycle_test()
{
    using enum Currency;
    
    ForeignExchange exchange{};
    
    exchange.add(USD, GBP, 0.76153);
    
    exchange.add(GBP, RON, 5.8098);
    
    exchange.add(RON, USD, 0.2168);
    
    return exchange.get_all_fx_rates(USD, RON).front() - 4.42434 < 0.00001;
}

bool multiple_rates_test()
{
    using enum Currency;
    
    ForeignExchange exchange{};
    
    exchange.add(USD, RON, 4.60288);
    exchange.add(USD, GBP, 0.76153);
    
    exchange.add(GBP, RON, 5.8098);
    
    std::vector<double> received_response{exchange.get_all_fx_rates(USD, RON)};
    std::vector<double> correct_response{4.60288, 4.42434};
    
    for (size_t i = 0; i < correct_response.size(); ++i)
    {
        if (received_response[i] - correct_response[i] >= 0.00001)
        {
            return false;
        }
    }
    
    return true;
}

bool cycle_and_multiple_rates_test()
{
    using enum Currency;
    
    ForeignExchange exchange{};
    
    exchange.add(USD, RON, 4.60288);
    exchange.add(USD, CAD, 1.35438);
    exchange.add(USD, MEX, 16.5772);
    
    exchange.add(CAD, USD, 0.7382);
    exchange.add(CAD, MEX, 12.2372);
    exchange.add(CAD, GBP, 0.58468);
    
    exchange.add(MEX, USD, 0.06028);
    exchange.add(MEX, CAD, 0.08164);
    
    exchange.add(GBP, RON, 5.8098);
    
    exchange.add(RON, USD, 0.2168);
    exchange.add(RON, GBP, 0.17171);
    
    std::vector<double> received_response{exchange.get_all_fx_rates(USD, RON)};
    std::vector<double> correct_response{4.60288, 4.60066};
    
    for (size_t i = 0; i < correct_response.size(); ++i)
    {
        if (received_response[i] - correct_response[i] >= 0.00001)
        {
            return false;
        }
    }
    
    return true;
}

bool foreign_exchange_tests()
{
    return empty_test()
        && remove_test()
        && simple_cycle_test()
        && multiple_rates_test()
        && cycle_and_multiple_rates_test();
}