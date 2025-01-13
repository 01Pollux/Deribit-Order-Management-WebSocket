#pragma once

#include <order-management/order-desc.hpp>
#include <boost/property_tree/ptree.hpp>

#include "common-json-map.hpp"

namespace om
{
    struct AuthMapper
    {
        static JsonMap CreateAuthRequest()
        {
            return CommonJsonMap::Create(config::AuthWSMethod);
        }

        static JsonMap RefreshAuthRequest(const std::string& refreshToken)
        {
            auto message = CommonJsonMap::Create(config::AuthWSMethod);

            auto& params = message.get_child("params");
            params.add("grant_type", "refresh_token");
            params.add("refresh_token", refreshToken);

            return message;
        }
    };
} // namespace om