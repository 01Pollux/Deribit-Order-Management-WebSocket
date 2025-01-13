#pragma once

#include <order-management/json-map.hpp>
#include <order-management/config.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace om
{
    struct CommonJsonMap
    {
        /// <summary>
        /// Create a message with the given id and method type
        /// </summary>
        static JsonMap Create(const config::MethodAndId& methodAndId)
        {
            JsonMap message;
            InitializeHeader(message, methodAndId.Id, methodAndId.Method);
            InitializeParams(message.put_child("params", {}), config::DeribitClientId, config::DeribitClientSecret);
            return message;
        }

        /// <summary>
        /// Initialize the header for the message
        /// </summary>
        static void InitializeHeader(JsonMap& message, int id, const char* methodType)
        {
            message.add("jsonrpc", "2.0");
            message.add("id", id);
            message.add("method", methodType);
        }

        /// <summary>
        /// Initialize the parameters for the message
        /// </summary>
        static void InitializeParams(JsonMap& params, const char* clientId, const char* clientSecret)
        {
            params.add("grant_type", "client_credentials");
            params.add("client_id", clientId);
            params.add("client_secret", clientSecret);
        }

        /// <summary>
        /// Helper function to print the message on debug
        /// </summary>
        static void Print(const JsonMap& message)
        {
            if constexpr (config::DebugPrints)
            {
                spdlog::apply_all([&](auto logger) { logger->info("Sending message: {}", ToString(message)); });
            }
        }

        /// <summary>
        /// Helper function to see if the response contains an error and print it
        /// </summary>
        /// <returns>true if the response contains an error, false otherwise</returns>
        static bool PrintOnError(const JsonMap& response)
        {
            auto errorIter = response.find("error");
            if (errorIter == response.not_found())
            {
                return false;
            }

            if constexpr (config::DebugPrints)
            {
                int         code    = errorIter->second.get<int>("code");
                std::string message = errorIter->second.get<std::string>("message");
                std::string reason  = errorIter->second.get<std::string>("data.reason", INTERNAL_ERROR_STRING);

                spdlog::apply_all(
                    [&](auto logger) {
                        logger->error("Error code: {}, message: {}, reason: {}, Details: {}", code, message, reason,
                                      ToString(response));
                    });
            }

            return true;
        }
    };
} // namespace om