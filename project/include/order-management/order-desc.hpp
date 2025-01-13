#pragma once

#include "core.hpp"

namespace om
{
    enum class OrderDirection : uint8_t
    {
        Buy,
        Sell
    };

    enum class InstrumentType : uint8_t
    {
        Spot,
        Futures,
        Options
    };

    enum class OrderType : uint8_t
    {
        Limit,
        StopLimit,
        TakeLimit,
        Market,
        StopMarket,
        TakeMarket,
        MarketLimit,
        TrailStop,
    };

    enum class OrderRemainEffect : uint8_t
    {
        GoodTilCancelled,
        GoodTilDay,
        FillOrKill,
        ImmediateOrCancel,
    };

    enum class OrderPriceTrigger : uint8_t
    {
        IndexPrice,
        MarkPrice,
        LastPrice,
    };

    enum class OrderAdvancedOption : uint8_t
    {
        USD,
        IMPLV
    };

    struct PlaceOrderDesc
    {
        std::string Symbol;

        std::optional<double> Amount;

        std::optional<OrderDirection> Direction;

        std::optional<OrderType> Type;

        std::string Label;

        std::optional<double> Price;

        std::optional<bool> ReduceOnly;

        std::optional<OrderRemainEffect> TimeInForce;

        std::optional<bool> PostOnly;

        std::optional<bool> RejectPostOnly;

        std::optional<double> TriggerPrice;

        std::optional<double> TriggerOffset;

        std::optional<OrderPriceTrigger> Trigger;
    };

    struct ModifyOrderDesc
    {
        std::optional<double> Amount;

        std::optional<double> Price;

        std::optional<bool> PostOnly;

        std::optional<bool> ReduceOnly;

        std::optional<bool> RejectPostOnly;

        std::optional<OrderAdvancedOption> Advanced;

        std::optional<double> TriggerPrice;

        std::optional<double> TriggerOffset;
    };
} // namespace om