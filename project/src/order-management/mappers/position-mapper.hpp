#pragma once

#include <unordered_map>
#include <order-management/position.hpp>

#include "common-json-map.hpp"

namespace om
{
    struct PositionMapper
    {
        static inline const std::unordered_map<std::string, PositionDirection> PositionDirectionMap{
            { "buy", PositionDirection::Buy }, { "sell", PositionDirection::Sell }, { "zero", PositionDirection::Zero }
        };

        static JsonMap MapToRequestForPosition(const std::string& symbol)
        {
            auto message = CommonJsonMap::Create(config::GetPositionWSMethod);

            auto& params = message.get_child("params");
            params.put("instrument_name", symbol);

            return message;
        }

        static JsonMap MapToRequestForPositions(const GetPositionsDesc& depth)
        {
            auto message = CommonJsonMap::Create(config::GetPositionsWSMethod);

            auto& params = message.get_child("params");

            params.put("currency", depth.Currency);

            if (depth.Kind.has_value())
            {
                switch (*depth.Kind)
                {
                case PositionKind::Future:
                    params.put("kind", "future");
                    break;
                case PositionKind::Option:
                    params.put("kind", "option");
                    break;
                case PositionKind::Spot:
                    params.put("kind", "spot");
                    break;
                case PositionKind::FutureCombo:
                    params.put("kind", "future_combo");
                    break;
                case PositionKind::OptionCombo:
                    params.put("kind", "option_combo");
                    break;
                }
            }

            if (depth.SubAccountId.has_value())
            {
                params.put("subaccount", depth.SubAccountId.value());
            }

            return message;
        }

        static std::vector<PositionDesc> MapToPositions(const JsonMap& response)
        {
            auto& result = response.get_child("result");

            std::vector<PositionDesc> positions;
            positions.reserve(result.size());

            for (const auto& [key, value] : result)
            {
                positions.emplace_back(MapOneToPosition(value));
            }

            return positions;
        }

        static PositionDesc MapToPosition(const JsonMap& response)
        {
            auto& result = response.get_child("result");
            return MapOneToPosition(result);
        }

    private:
        static PositionDesc MapOneToPosition(const JsonMap& result)
        {
            auto x = result.get<std::string>("average_price");

            return PositionDesc{
                .AveragePrice       = result.get<double>("average_price"),
                .Delta              = result.get<double>("delta"),
                .Direction          = PositionDirectionMap.at(result.get<std::string>("direction")),
                .FloatingProfitLoss = result.get<double>("floating_profit_loss"),
                .RealizedProfitLoss = result.get<double>("realized_profit_loss"),
                .TotalProfitLoss    = result.get<double>("total_profit_loss"),
                .Size               = result.get<double>("size"),
            };
        }
    };
} // namespace om