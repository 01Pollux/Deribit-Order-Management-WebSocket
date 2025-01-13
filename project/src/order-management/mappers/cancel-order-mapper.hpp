#pragma once

#include <order-management/order-desc.hpp>
#include <boost/property_tree/ptree.hpp>

#include "common-json-map.hpp"

namespace om
{
    struct CancelOrderMapper
    {
        static JsonMap MapToRequest(const std::string& orderId)
        {
            auto message = CommonJsonMap::Create(config::CancelOrderWSMethod);

            auto& params = message.get_child("params");
            params.put("order_id", orderId);

            return message;
        }
    };
} // namespace om