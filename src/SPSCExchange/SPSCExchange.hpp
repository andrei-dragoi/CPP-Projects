#pragma once

#include <thread>

#include "../Exchange/Exchange.hpp"
#include "../RingBuffer/RingBuffer.hpp"

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

template<size_t size>
class SPSCExchange
{
public:
    SPSCExchange(TradeRecorder& trade_recorder, RandomOrderGenerator& random_order_generator, Exchange& external_exchange)
    : _exchange{trade_recorder}
    , _keep_going{true}
    , _producer_thread{ [&](){ this->produce(random_order_generator); } }
    , _consumer_thread{ [&](){ this->consume(external_exchange); } }
    {
    }

    void produce(RandomOrderGenerator& random_order_generator)
    {
        Order order{};

        while (_keep_going.load(std::memory_order_relaxed))
        {
            order = random_order_generator.generate();

            while (_keep_going.load(std::memory_order_relaxed) && !_ring_buffer.write(order))
            {
            }
        }
    }

    void consume(Exchange& external_exchange)
    {
        Order order{};

        while (_keep_going.load(std::memory_order_relaxed))
        {
            while (_keep_going.load(std::memory_order_relaxed) && _ring_buffer.read(order))
            { 
                _exchange.add_order(order);
                external_exchange.add_order(order);
            }
        }
    }

    void terminate()
    {
        _keep_going.store(false, std::memory_order_acq_rel);
    }

private:
    Exchange _exchange;
    RingBuffer<Order, size> _ring_buffer{};
    std::atomic<bool> _keep_going{};
    std::jthread _producer_thread{};
    std::jthread _consumer_thread{};
};