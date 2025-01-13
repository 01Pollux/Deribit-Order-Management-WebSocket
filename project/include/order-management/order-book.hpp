#pragma once

#include <vector>

namespace om
{
    enum class OrderBookDepth : int32_t
    {
        Invalid = -1,
        _1      = 1,
        _5      = 5,
        _10     = 10,
        _20     = 20,
        _50     = 50,
        _100    = 100,
        _1000   = 1000,
        _10000  = 10000,
    };

    struct OrderBook
    {
        enum class State : uint8_t
        {
            Open,
            Closed
        };

        struct Ask
        {
            double Price;
            double Amount;
        };

        struct Bid
        {
            double Price;
            double Amount;
        };

        std::vector<Ask> Asks;
        std::vector<Bid> Bids;

        Ask BestAsk;
        Bid BestBid;

        State CurrentState = State::Closed;
    };
} // namespace om