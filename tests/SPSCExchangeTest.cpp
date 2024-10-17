#include <vector>
#include <chrono>

#include "../src/Exchange/Exchange.hpp"
#include "../src/Exchange/TradeRecorder.hpp"
#include "../src/SPSCExchange/SPSCExchange.hpp"

bool spsc_random_data_test(size_t iters)
{
    TradeRecorder internal_trade_recorder{};
    TradeRecorder external_trade_recorder{};
    
    std::vector<Exchange> exchange_list {Exchange{internal_trade_recorder}, Exchange{external_trade_recorder}};
    
    RandomOrderGenerator random_order_generator{2, 10, 100};

    std::vector<Order> order_list{};
    for (size_t i = 0; i < iters; ++i)
    {
        order_list.emplace_back(random_order_generator.generate());
    }
    
    SPSCExchange spsc_exchange{exchange_list, order_list};
    
    //wait time
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    
    spsc_exchange.terminate();
    
    return internal_trade_recorder.record() == external_trade_recorder.record();
}

bool multiple_spsc_random_data_test()
{
    size_t iters = 128;
    
    for (size_t i = 0; i < 5; ++i)
    {
        if (!spsc_random_data_test(iters))
        {
            return false;
        }
        
        i <<= 1;
    }
    
    return true;
}

bool spsc_all_tests()
{
    return multiple_spsc_random_data_test();
}