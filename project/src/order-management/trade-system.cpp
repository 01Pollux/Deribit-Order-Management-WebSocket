#include <order-management/trade-system.hpp>

#include "mappers/auth-mapper.hpp"
#include "mappers/place-order-mapper.hpp"
#include "mappers/cancel-order-mapper.hpp"
#include "mappers/modify-order-mapper.hpp"
#include "mappers/order-book-mapper.hpp"
#include "mappers/position-mapper.hpp"

#include <order-management/benchmarking.hpp>

namespace om
{
    TradeSystem::TradeSystem() : m_AccessTokenHandler(m_DeribitWSClient)
    {
    }

    //

    bool TradeSystem::PlaceOrder(const PlaceOrderDesc& orderDesc, std::string* orderId)
    {
        ZONE_BENCHMARK("TradeSystem Place Order");

        if (!m_AccessTokenHandler.TryToAuthenticate())
        {
            return false;
        }

        auto request  = PlaceOrderMapper::MapToRequest(orderDesc);
        auto response = m_DeribitWSClient.Send(request);
        if (CommonJsonMap::PrintOnError(response))
        {
            return false;
        }

        CommonJsonMap::Print(response);
        if (orderId)
        {
            *orderId = response.get<std::string>("result.order.order_id");
        }
        return true;
    }

    //

    bool TradeSystem::CancelOrder(const std::string& orderId)
    {
        ZONE_BENCHMARK("TradeSystem Cancel Order");

        if (!m_AccessTokenHandler.TryToAuthenticate())
        {
            return false;
        }

        auto request  = CancelOrderMapper::MapToRequest(orderId);
        auto response = m_DeribitWSClient.Send(request);
        if (CommonJsonMap::PrintOnError(response))
        {
            return false;
        }

        CommonJsonMap::Print(response);
        return true;
    }

    //

    bool TradeSystem::ModifyOrder(const std::string& orderId, const ModifyOrderDesc& orderDesc)
    {
        ZONE_BENCHMARK("TradeSystem Modify Order");

        if (!m_AccessTokenHandler.TryToAuthenticate())
        {
            return false;
        }

        auto request  = ModifyOrderMapper::MapToRequest(orderId, orderDesc);
        auto response = m_DeribitWSClient.Send(request);
        if (CommonJsonMap::PrintOnError(response))
        {
            return false;
        }

        CommonJsonMap::Print(response);
        return true;
    }

    //

    std::optional<OrderBook> TradeSystem::GetOrderBook(const std::string& symbol, OrderBookDepth depth)
    {
        ZONE_BENCHMARK("TradeSystem Get Order Book");

        auto request  = OrderBookMapper::MapToRequest(symbol, depth);
        auto response = m_DeribitWSClient.Send(request);
        if (CommonJsonMap::PrintOnError(response))
        {
            return std::nullopt;
        }

        CommonJsonMap::Print(response);
        return OrderBookMapper::MapToOrderBook(response);
    }

    //

    std::optional<PositionDesc> TradeSystem::GetPosition(const std::string& symbol)
    {
        ZONE_BENCHMARK("TradeSystem Get Position");

        std::optional<PositionDesc> position;

        if (!m_AccessTokenHandler.TryToAuthenticate())
        {
            return position;
        }

        auto request  = PositionMapper::MapToRequestForPosition(symbol);
        auto response = m_DeribitWSClient.Send(request);
        if (CommonJsonMap::PrintOnError(response))
        {
            return position;
        }

        CommonJsonMap::Print(response);
        position.emplace(PositionMapper::MapToPosition(response));

        return position;
    }

    std::optional<std::vector<PositionDesc>> TradeSystem::GetPositions(const GetPositionsDesc& positionDesc)
    {
        ZONE_BENCHMARK("TradeSystem Get Positions");

        std::optional<std::vector<PositionDesc>> positions;

        if (!m_AccessTokenHandler.TryToAuthenticate())
        {
            return positions;
        }

        auto request  = PositionMapper::MapToRequestForPositions(positionDesc);
        auto response = m_DeribitWSClient.Send(request);
        if (CommonJsonMap::PrintOnError(response))
        {
            return positions;
        }

        CommonJsonMap::Print(response);
        positions.emplace(PositionMapper::MapToPositions(response));

        return positions;
    }
} // namespace om