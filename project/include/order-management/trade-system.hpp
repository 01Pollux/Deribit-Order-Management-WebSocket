#pragma once

#include "core.hpp"
#include "deribit-ws-client.hpp"
#include "order-book.hpp"
#include "position.hpp"
#include "access-token-handler.hpp"

namespace om
{
    class TradeSystem
    {
    public:
        TradeSystem();

    public:
        /// <summary>
        /// Place an order in the system
        /// </summary>
        /// <param name="newOrderDesc">the order to place</param>
        /// <param name="orderId">optional output parameter to receive the order id</param>
        bool PlaceOrder(const PlaceOrderDesc& orderDesc, std::string* orderId = nullptr);

        /// <summary>
        /// Cancel an order in the system
        /// </summary>
        bool CancelOrder(const std::string& orderId);

        /// <summary>
        /// Modify an order in the system
        /// </summary>
        bool ModifyOrder(const std::string& orderId, const ModifyOrderDesc& orderDesc);

        /// <summary>
        /// Get the order book for a symbol
        /// </summary>
        [[nodiscard]] std::optional<OrderBook> GetOrderBook(const std::string& symbol, OrderBookDepth depth);

        /// <summary>
        /// Get position for a symbol
        /// </summary>
        [[nodiscard]] std::optional<PositionDesc> GetPosition(const std::string& symbol);

        /// <summary>
        /// Get positions for a symbol
        /// </summary>
        [[nodiscard]] std::optional<std::vector<PositionDesc>> GetPositions(const GetPositionsDesc& positionDesc);

    private:
        DeribitWSClient    m_DeribitWSClient;
        AccessTokenHandler m_AccessTokenHandler;
    };
} // namespace om