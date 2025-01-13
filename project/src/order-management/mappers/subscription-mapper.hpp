#pragma once

#include <boost/property_tree/ptree.hpp>

#include "common-json-map.hpp"

namespace om
{
    struct SubscriptionMapper
    {
        static JsonMap MapToSubscribeRequest(const std::string& symbol)
        {
            auto message = CommonJsonMap::Create(config::SubscribeWSMethod);

            auto& channels = message.put_child("params.channels", {});
            channels.push_back(std::make_pair("", boost::property_tree::ptree(symbol)));

            return message;
        }

        static JsonMap MapToUnsubscribeRequest(const std::string& symbol)
        {
            auto message = CommonJsonMap::Create(config::UnsubscribeWSMethod);

            auto& channels = message.put_child("params.channels", {});
            channels.push_back(std::make_pair("", boost::property_tree::ptree(symbol)));

            return message;
        }
    };
} // namespace om