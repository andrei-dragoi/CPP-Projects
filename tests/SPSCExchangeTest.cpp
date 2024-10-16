#include <vector>
#include <chrono>

#include "../src/Exchange/Exchange.hpp"
#include "../src/Exchange/TradeRecorder.hpp"
#include "../src/SPSCExchange/SPSCExchange.hpp"

bool compare_transaction_vectors(const std::vector<Transaction>& lhs, const std::vector<Transaction>& rhs);

bool spsc_random_data_test()
{
    TradeRecorder internal_trade_recorder{};
    TradeRecorder external_trade_recorder{};
    
    Exchange external_exchange{external_trade_recorder};
    
    RandomOrderGenerator random_order_generator{2, 10, 100};
    
    SPSCExchange spsc_exchange{internal_trade_recorder, random_order_generator, external_exchange};
    
    //wait time
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
    spsc_exchange.terminate();
    
    return compare_transaction_vectors(internal_trade_recorder.record(), external_trade_recorder.record());
}

bool spsc_all_tests()
{
    return spsc_random_data_test();
}