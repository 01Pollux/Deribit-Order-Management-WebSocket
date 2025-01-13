#pragma once

#include <order-management/core.hpp>

namespace om
{
    enum class PositionDirection : uint8_t
    {
        Buy,
        Sell,
        Zero
    };

    enum class PositionKind : uint8_t
    {
        Future,
        Option,
        Spot,
        FutureCombo,
        OptionCombo,
    };

    struct GetPositionsDesc
    {
        std::string Currency;

        std::optional<PositionKind> Kind;

        std::optional<int> SubAccountId;
    };

    struct PositionDesc
    {
        double AveragePrice;

        double Delta;

        PositionDirection Direction;

        double FloatingProfitLoss;

        double RealizedProfitLoss;

        double TotalProfitLoss;

        double Size;
    };
} // namespace om