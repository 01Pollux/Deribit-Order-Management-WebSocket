#pragma once

namespace om::config
{
    struct MethodAndId
    {
        const char* Method = nullptr;
        int         Id     = -1;
    };

#if OM_DEBUG
    inline constexpr bool DebugPrints = true;
#else
    inline constexpr bool DebugPrints = false;
#endif

    //

    inline constexpr const char* DeribitClientId     = "your_client_id";
    inline constexpr const char* DeribitClientSecret = "your_client_secret";

    //

    inline constexpr const char* DeribitHost          = "test.deribit.com";
    inline constexpr const char* DeribitPort          = "443";
    inline constexpr const char* DeribitWSApiEndPoint = "/ws/api/v2";

    //

    inline constexpr MethodAndId AuthWSMethod{ "public/auth", 9929 };

    inline constexpr MethodAndId CancelOrderWSMethod{ "private/cancel", 4214 };
    inline constexpr MethodAndId ModifyOrderWSMethod{ "private/edit", 3725 };
    inline constexpr MethodAndId GetOrderBookWSMethod{ "public/get_order_book", 8772 };
    inline constexpr MethodAndId PlaceOrderWSMethod{ "private/buy", 5275 };
    inline constexpr MethodAndId SellOrderWSMethod{ "private/sell", 2148 };
    inline constexpr MethodAndId GetPositionWSMethod{ "private/get_position", 404 };
    inline constexpr MethodAndId GetPositionsWSMethod{ "private/get_positions", 2236 };
    inline constexpr MethodAndId SubscribeWSMethod{ "private/subscribe", 4235 };
    inline constexpr MethodAndId UnsubscribeWSMethod{ "private/unsubscribe", 3370 };
} // namespace om::config