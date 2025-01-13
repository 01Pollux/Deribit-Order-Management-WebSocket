#pragma once

#include <unordered_map>
#include <order-management/order-book.hpp>
#include <boost/property_tree/ptree.hpp>

#include "common-json-map.hpp"

namespace om
{
    struct OrderBookMapper
    {
        static inline const std::unordered_map<std::string, OrderBook::State> OrderBookStateMap{
            { "open", OrderBook::State::Open }, { "closed", OrderBook::State::Closed }
        };

        static JsonMap MapToRequest(const std::string& symbol, OrderBookDepth depth)
        {
            auto message = CommonJsonMap::Create(config::GetOrderBookWSMethod);

            auto& params = message.get_child("params");
            params.put("instrument_name", symbol);
            params.put("depth", std::to_underlying(depth));

            return message;
        }

        static OrderBook MapToOrderBook(const JsonMap& response)
        {
            auto& result = response.get_child("result");

            OrderBook orderBook;

            for (const auto& [key, arrayValue] : result.get_child("asks"))
            {
                auto arrayIter = arrayValue.begin();

                auto price  = arrayIter->second.get_value<double>();
                auto amount = std::next(arrayIter)->second.get_value<double>();

                orderBook.Asks.push_back({ .Price = price, .Amount = amount });
            }

            for (const auto& [key, arrayValue] : result.get_child("bids"))
            {
                auto arrayIter = arrayValue.begin();

                auto price  = arrayIter->second.get_value<double>();
                auto amount = std::next(arrayIter)->second.get_value<double>();

                orderBook.Asks.push_back({ .Price = price, .Amount = amount });
            }

            orderBook.BestAsk = {
                .Price  = result.get<double>("best_ask_price"),
                .Amount = result.get<double>("best_ask_amount"),
            };

            orderBook.BestBid = {
                .Price  = result.get<double>("best_bid_price"),
                .Amount = result.get<double>("best_bid_amount"),
            };

            orderBook.CurrentState = OrderBookStateMap.at(result.get<std::string>("state"));

            return orderBook;
        }
    };
} // namespace om