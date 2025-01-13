#pragma once

#include <order-management/order-desc.hpp>
#include <boost/property_tree/ptree.hpp>

#include "common-json-map.hpp"

namespace om
{
    struct PlaceOrderMapper
    {
        static JsonMap MapToRequest(const PlaceOrderDesc& order)
        {
            auto message = CommonJsonMap::Create(order.Direction == OrderDirection::Buy ? config::PlaceOrderWSMethod
                                                                                        : config::SellOrderWSMethod);

            auto& params = message.get_child("params");
            params.put("instrument_name", order.Symbol);
            if (order.Amount)
            {
                params.put("amount", *order.Amount);
            }

            if (order.Direction)
            {
                params.put("direction", order.Direction == OrderDirection::Buy ? "buy" : "sell");
            }

            if (order.Type)
            {
                const char* type = nullptr;
                switch (*order.Type)
                {
                case OrderType::Limit:
                    type = "limit";
                    break;
                case OrderType::StopLimit:
                    type = "stop_limit";
                    break;
                case OrderType::TakeLimit:
                    type = "take_limit";
                    break;
                case OrderType::Market:
                    type = "market";
                    break;
                case OrderType::StopMarket:
                    type = "stop_market";
                    break;
                case OrderType::TakeMarket:
                    type = "take_market";
                    break;
                case OrderType::MarketLimit:
                    type = "market_limit";
                    break;
                case OrderType::TrailStop:
                    type = "trail_stop";
                    break;
                }
                params.put("type", type);
            }

            if (!order.Label.empty())
            {
                params.put("label", order.Label);
            }

            if (order.Price)
            {
                params.put("price", *order.Price);
            }

            if (order.ReduceOnly)
            {
                params.put("reduce_only", *order.ReduceOnly);
            }

            if (order.TimeInForce)
            {
                const char* tif = nullptr;
                switch (*order.TimeInForce)
                {
                case OrderRemainEffect::GoodTilCancelled:
                    tif = "good_til_cancelled";
                    break;
                case OrderRemainEffect::GoodTilDay:
                    tif = "good_til_day";
                    break;
                case OrderRemainEffect::FillOrKill:
                    tif = "fill_or_kill";
                    break;
                case OrderRemainEffect::ImmediateOrCancel:
                    tif = "immediate_or_cancel";
                    break;
                }
                params.put("time_in_force", tif);
            }

            if (order.PostOnly)
            {
                params.put("post_only", *order.PostOnly);
            }

            if (order.RejectPostOnly)
            {
                params.put("reject_post_only", *order.RejectPostOnly);
            }

            if (order.TriggerPrice)
            {
                params.put("trigger_price", *order.TriggerPrice);
            }

            if (order.TriggerOffset)
            {
                params.put("trigger_offset", *order.TriggerOffset);
            }

            if (order.Trigger)
            {
                const char* trigger = nullptr;
                switch (*order.Trigger)
                {
                case OrderPriceTrigger::IndexPrice:
                    trigger = "index_price";
                    break;
                case OrderPriceTrigger::MarkPrice:
                    trigger = "mark_price";
                    break;
                case OrderPriceTrigger::LastPrice:
                    trigger = "last_price";
                    break;
                }
                params.put("trigger", trigger);
            }

            return message;
        }
    };
} // namespace om