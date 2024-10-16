
#include <string>
#include <unordered_map>
#include <vector>
#include <cassert>

#include "../src/Exchange/Order.hpp"
#include "../src/Exchange/Exchange.hpp"
#include "../src/Exchange/TradeRecorder.hpp"

bool compare_transaction_vectors(const std::vector<Transaction>& lhs, const std::vector<Transaction>& rhs) {
    if (lhs.size() != rhs.size()) 
    {
        return false;
    }

    for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) 
        {
            return false; 
        }
    }
    return true;
}

bool basic_test()
{
    TradeRecorder recorder{};
    Exchange exchange{recorder};

    assert(recorder.record().empty());
    recorder.clear();

    std::string symbol_str = "AAPL";
    std::uint64_t symbol = std::hash<std::string>{}(symbol_str);  // Convert string to uint64_t

    exchange.add_order(Order{0, symbol, 110, 10, Side::bid});
    exchange.add_order(Order{1, symbol, 109, 10, Side::bid});
    exchange.add_order(Order{2, symbol, 111, 5, Side::ask});
    exchange.add_order(Order{3, symbol, 110, 5, Side::ask});

    assert(compare_transaction_vectors(
        recorder.record(),
        std::vector<Transaction>{
            Transaction{0, symbol, 110, 5},
            Transaction{3, symbol, 110, 5}
        }
    ));
    recorder.clear();

    exchange.add_order(Order{4, symbol, 110, 5, Side::ask});
    assert(compare_transaction_vectors(
        recorder.record(),
        std::vector<Transaction>{
            Transaction{0, symbol, 110, 5},
            Transaction{4, symbol, 110, 5}
        }
    ));
    recorder.clear();

    exchange.remove_order(2, symbol);

    exchange.add_order(Order{5, symbol, 109, 10, Side::bid});
    assert(recorder.record().empty());
    recorder.clear();

    exchange.add_order(Order{6, symbol, 110, 10, Side::bid});
    assert(recorder.record().empty());
    recorder.clear();

    exchange.add_order(Order{7, symbol, 111, 10, Side::ask});
    assert(recorder.record().empty());
    recorder.clear();

    exchange.add_order(Order{8, symbol, 112, 10, Side::ask});
    assert(recorder.record().empty());
    recorder.clear();

    exchange.add_order(Order{9, symbol, 112, 10, Side::ask});
    assert(recorder.record().empty());
    recorder.clear();

    exchange.add_order(Order{10, symbol, 112, 13, Side::bid});
    assert(compare_transaction_vectors(
        recorder.record(),
        std::vector<Transaction>{
            Transaction{7, symbol, 111, 10},
            Transaction{10, symbol, 111, 10},
            Transaction{8, symbol, 112, 3},
            Transaction{10, symbol, 112, 3}
        }
    ));
    recorder.clear();

    return true;
}

bool partial_book_removal_test()
{
    TradeRecorder recorder{};
    Exchange exchange{recorder};

    std::string symbol_str = "AAPL";
    std::uint64_t symbol = std::hash<std::string>{}(symbol_str);

    exchange.add_order(Order{0, symbol, 110, 10, Side::bid});
    exchange.add_order(Order{1, symbol, 110, 5, Side::ask});

    assert(compare_transaction_vectors(
        recorder.record(),
        std::vector<Transaction>{
            Transaction{0, symbol, 110, 5},
            Transaction{1, symbol, 110, 5}
        }
    ));
    recorder.clear();

    exchange.remove_order(0, symbol);
    exchange.add_order(Order{2, symbol, 110, 5, Side::ask});

    assert(recorder.record().empty());
    recorder.clear();

    return true;
}

bool fifo_fill_matching_test()
{
    TradeRecorder recorder{};
    Exchange exchange{recorder};

    std::string symbol_str = "AAPL";
    std::uint64_t symbol = std::hash<std::string>{}(symbol_str);

    exchange.add_order(Order{0, symbol, 110, 10, Side::bid});
    exchange.add_order(Order{1, symbol, 110, 10, Side::bid});
    exchange.add_order(Order{2, symbol, 110, 10, Side::bid});
    exchange.add_order(Order{3, symbol, 110, 10, Side::ask});

    assert(compare_transaction_vectors(
        recorder.record(),
        std::vector<Transaction>{
            Transaction{0, symbol, 110, 10},
            Transaction{3, symbol, 110, 10}
        }
    ));
    recorder.clear();

    exchange.add_order(Order{4, symbol, 110, 10, Side::bid});
    exchange.add_order(Order{5, symbol, 110, 10, Side::ask});

    assert(compare_transaction_vectors(
        recorder.record(),
        std::vector<Transaction>{
            Transaction{1, symbol, 110, 10},
            Transaction{5, symbol, 110, 10}
        }
    ));
    recorder.clear();

    exchange.remove_order(2, symbol);
    exchange.add_order(Order{6, symbol, 110, 10, Side::ask});

    assert(compare_transaction_vectors(
        recorder.record(),
        std::vector<Transaction>{
            Transaction{4, symbol, 110, 10},
            Transaction{6, symbol, 110, 10}
        }
    ));
    recorder.clear();

    return true;
}

bool most_competitive_opposite_price_test()
{
    TradeRecorder recorder{};
    Exchange exchange{recorder};
    std::uint64_t symbol = std::hash<std::string>{}("IBM");

    exchange.add_order(Order{0, symbol, 110, 10, Side::bid});
    exchange.add_order(Order{1, symbol, 111, 10, Side::bid});
    exchange.add_order(Order{2, symbol, 110, 10, Side::bid});
    exchange.add_order(Order{3, symbol, 110, 10, Side::ask});

    assert(compare_transaction_vectors(
        recorder.record(),
        std::vector<Transaction>{
            Transaction{1, symbol, 111, 10},
            Transaction{3, symbol, 111, 10}
        }
    ));
    recorder.clear();

    return true;
}

bool multiple_fills_test()
{
    TradeRecorder recorder{};
    Exchange exchange{recorder};
    std::uint64_t symbol = std::hash<std::string>{}("IBM");

    exchange.add_order(Order{0, symbol, 111, 10, Side::bid});
    exchange.add_order(Order{1, symbol, 110, 10, Side::bid});
    exchange.add_order(Order{2, symbol, 109, 10, Side::bid});
    exchange.add_order(Order{3, symbol, 109, 25, Side::ask});

    assert(compare_transaction_vectors(
        recorder.record(),
        std::vector<Transaction>{
            Transaction{0, symbol, 111, 10},
            Transaction{3, symbol, 111, 10},
            Transaction{1, symbol, 110, 10},
            Transaction{3, symbol, 110, 10},
            Transaction{2, symbol, 109, 5},
            Transaction{3, symbol, 109, 5}
        }
    ));
    recorder.clear();

    return true;
}

bool multiple_symbols_test()
{
    TradeRecorder recorder{};
    Exchange exchange{recorder};

    std::uint64_t symbol1 = std::hash<std::string>{}("IBM");
    std::uint64_t symbol2 = std::hash<std::string>{}("APPL");

    exchange.add_order(Order{0, symbol1, 110, 10, Side::bid});
    exchange.add_order(Order{1, symbol2, 110, 10, Side::ask});

    assert(compare_transaction_vectors(recorder.record(), {}));
    recorder.clear();

    exchange.add_order(Order{2, symbol2, 110, 10, Side::bid});

    assert(compare_transaction_vectors(
        recorder.record(),
        std::vector<Transaction>{
            Transaction{1, symbol2, 110, 10},
            Transaction{2, symbol2, 110, 10}
        }
    ));
    recorder.clear();

    return true;
}

bool run_all_tests() {
    std::vector<std::pair<std::string, bool(*)()>> tests = {
        {"Basic test", basic_test},
        {"Partial book removal test", partial_book_removal_test},
        {"FIFO simple cross fill matching test", fifo_fill_matching_test},
        {"Most competitive oppiste order book price test", most_competitive_opposite_price_test},
        {"Multiple fills test", multiple_fills_test},
        {"Multiple symbols test", multiple_symbols_test}
    };

    for (const auto& test : tests) {
        const auto& [name, test_func] = test;
        try {
            assert(test_func());
            std::cout << name << ": PASSED\n";
        } catch (const std::exception& e) {
            std::cerr << name << ": FAILED - " << e.what() << '\n';
            return false;
        }
    }
    
    return true;
}