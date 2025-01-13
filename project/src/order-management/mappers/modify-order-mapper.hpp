#pragma once

#include <order-management/order-desc.hpp>
#include <boost/property_tree/ptree.hpp>

#include "common-json-map.hpp"

namespace om
{
    struct ModifyOrderMapper
    {
        static JsonMap MapToRequest(const std::string& orderId, const ModifyOrderDesc& order)
        {
            auto message = CommonJsonMap::Create(config::ModifyOrderWSMethod);

            auto& params = message.get_child("params");
            params.put("order_id", orderId);

            if (order.Amount)
            {
                params.put("amount", *order.Amount);
            }

            if (order.Price)
            {
                params.put("price", *order.Price);
            }

            if (order.PostOnly)
            {
                params.put("post_only", *order.PostOnly);
            }

            if (order.ReduceOnly)
            {
                params.put("reduce_only", *order.ReduceOnly);
            }

            if (order.RejectPostOnly)
            {
                params.put("reject_post_only", *order.RejectPostOnly);
            }

            if (order.Advanced)
            {
                const char* advanced = nullptr;
                switch (*order.Advanced)
                {
                case OrderAdvancedOption::USD:
                    advanced = "usd";
                    break;
                case OrderAdvancedOption::IMPLV:
                    advanced = "implv";
                    break;
                }
                params.put("advanced", advanced);
            }

            if (order.TriggerPrice)
            {
                params.put("trigger_price", *order.TriggerPrice);
            }

            if (order.TriggerOffset)
            {
                params.put("trigger_offset", *order.TriggerOffset);
            }

            return message;
        }
    };
} // namespace om