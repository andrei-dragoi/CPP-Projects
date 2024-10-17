#pragma once

#include <thread>
#include <vector>

#include "../Exchange/Exchange.hpp"
#include "../RingBuffer/RingBuffer.hpp"
#include "RandomOrderGenerator.hpp"

template<size_t buffer_size = 126, size_t exchange_count = 1>
class SPSCExchange
{
public:
    SPSCExchange(std::vector<Exchange>& exchange_list, RandomOrderGenerator& random_order_generator)
    : _ring_buffer{}
    , _keep_going{true}
    , _producer_thread{ [&](){ this->produce(random_order_generator); } }
    , _consumer_thread{ [&](){ this->consume(exchange_list); } }
    {
    }
    
    SPSCExchange(std::vector<Exchange>& exchange_list, const std::vector<Order>& order_list)
    : _ring_buffer{}
    , _keep_going{true}
    , _producer_thread{ [&](){ this->produce(order_list); } }
    , _consumer_thread{ [&](){ this->consume(exchange_list); } }
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

    void produce(const std::vector<Order>& orders)
    {
        auto order_iter = orders.begin();
        Order order{};

        while (_keep_going.load(std::memory_order_relaxed) && order_iter != orders.end())
        {
            order = *order_iter;

            while (_keep_going.load(std::memory_order_acq_rel) && !_ring_buffer.write(order))
            {
            }

            ++order_iter;
        }
    }

    void consume(std::vector<Exchange>& exchange_list)
    {
        Order order{};

        while (_keep_going.load(std::memory_order_relaxed))
        {
            while (_keep_going.load(std::memory_order_relaxed) && _ring_buffer.read(order))
            { 
                for (auto& exchange : exchange_list)
                {
                    exchange.add_order(order);
                }
            }
        }
    }

    void terminate()
    {
        _keep_going.store(false, std::memory_order_relaxed);
    }

private:
    RingBuffer<Order, buffer_size> _ring_buffer{};
    std::atomic<bool> _keep_going{};
    std::jthread _producer_thread{};
    std::jthread _consumer_thread{};
};