#define BOOST_TEST_MODULE LatencyBenchmarkingTest

#include <chrono>
#include <boost/test/included/unit_test.hpp>
#include <order-management/access-token-handler.hpp>
#include <order-management/notification-system.hpp>
#include <order-management/trade-system.hpp>
#include <order-management/order-desc.hpp>

using namespace om;
using namespace std::chrono;

BOOST_AUTO_TEST_SUITE(LatencyBenchmarkingSuite)

static constexpr uint32_t TryCount = 1;

BOOST_AUTO_TEST_CASE(OrderPlacementLatency)
{
    std::vector<long long> latencies;

    for (int i = 0; i < TryCount; i++)
    {
        TradeSystem tradeSystem;

        PlaceOrderDesc orderDesc;
        orderDesc.Symbol    = "ETH-PERPETUAL";
        orderDesc.Amount    = 1.0;
        orderDesc.Direction = OrderDirection::Buy;
        orderDesc.Type      = OrderType::Limit;
        orderDesc.Price     = 800.0;

        std::string orderId;

        auto start   = high_resolution_clock::now();
        bool success = tradeSystem.PlaceOrder(orderDesc, &orderId);
        auto end     = high_resolution_clock::now();

        BOOST_CHECK(success);

        if (success)
        {
            tradeSystem.CancelOrder(orderId);
        }

        auto duration = duration_cast<microseconds>(end - start).count();
        auto ns       = duration_cast<nanoseconds>(end - start).count();

        BOOST_TEST_MESSAGE("Order Placement Latency: " << duration << " microseconds (" << ns << " nanoseconds)");

        latencies.push_back(duration);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    std::sort(latencies.begin(), latencies.end());

    auto sum = std::accumulate(latencies.begin(), latencies.end(), 0.0);
    BOOST_TEST_MESSAGE("Average Order Placement Latency: " << sum / TryCount << " microseconds");
    BOOST_TEST_MESSAGE("Minimum Order Placement Loop Latency: " << latencies.front() << " microseconds");
    BOOST_TEST_MESSAGE("Maximum Order Placement Loop Latency: " << latencies.back() << " microseconds");
    BOOST_TEST_MESSAGE("Maximum Order Placement Loop 95th Percentile Latency: "
                       << latencies[int(latencies.size() * 0.95)] << " microseconds");
    BOOST_TEST_MESSAGE("Maximum Order Placement Loop 99th Percentile Latency: "
                       << latencies[int(latencies.size() * 0.99)] << " microseconds");
}

BOOST_AUTO_TEST_CASE(MarketDataProcessingLatency)
{
    std::vector<long long> latencies;

    for (int i = 0; i < TryCount; i++)
    {
        NotificationSystem notificationSystem;
        std::string        symbol = "platform_state";

        auto start   = high_resolution_clock::now();
        bool success = notificationSystem.Subscribe(symbol);
        auto end     = high_resolution_clock::now();

        BOOST_CHECK(success);

        auto duration = duration_cast<microseconds>(end - start).count();
        auto ns       = duration_cast<nanoseconds>(end - start).count();

        BOOST_TEST_MESSAGE("Market Data Processing Latency: " << duration << " microseconds (" << ns
                                                              << " nanoseconds)");

        latencies.push_back(duration);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    std::sort(latencies.begin(), latencies.end());

    auto sum = std::accumulate(latencies.begin(), latencies.end(), 0.0);
    BOOST_TEST_MESSAGE("Average Market Data Processing Loop Latency: " << sum / TryCount << " microseconds");
    BOOST_TEST_MESSAGE("Minimum Market Data Processing Loop Latency: " << latencies.front() << " microseconds");
    BOOST_TEST_MESSAGE("Maximum Market Data Processing Loop Latency: " << latencies.back() << " microseconds");
    BOOST_TEST_MESSAGE("Maximum Market Data Processing Loop 95th Percentile Latency: "
                       << latencies[int(latencies.size() * 0.95)] << " microseconds");
    BOOST_TEST_MESSAGE("Maximum Market Data Processing Loop 99th Percentile Latency: "
                       << latencies[int(latencies.size() * 0.99)] << " microseconds");
}

BOOST_AUTO_TEST_CASE(WebSocketMessagePropagationDelay)
{
    std::vector<long long> latencies0;
    std::vector<long long> latencies1;

    for (int i = 0; i < TryCount; i++)
    {
        DeribitWSClient wsClient;
        JsonMap         message;
        message.put("method", "public/test");

        auto start = high_resolution_clock::now();

        wsClient.Post(message);
        auto end0 = high_resolution_clock::now();

        auto response = wsClient.ReceiveNotification();
        auto end1     = high_resolution_clock::now();

        BOOST_CHECK(!response.empty());

        auto duration0 = duration_cast<microseconds>(end0 - start).count();
        auto duration1 = duration_cast<microseconds>(end1 - start).count();

        auto ns0 = duration_cast<nanoseconds>(end0 - start).count();
        auto ns1 = duration_cast<nanoseconds>(end1 - start).count();

        BOOST_TEST_MESSAGE("WebSocket Message Propagation Delay(Send): " << duration0 << " microseconds (" << ns0
                                                                         << " nanoseconds)");
        BOOST_TEST_MESSAGE("WebSocket Message Propagation Delay(Receive): " << duration1 << " microseconds (" << ns1
                                                                            << " nanoseconds)");

        latencies0.push_back(duration0);
        latencies1.push_back(duration1);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    std::sort(latencies0.begin(), latencies0.end());
    std::sort(latencies1.begin(), latencies1.end());

    auto sum0 = std::accumulate(latencies0.begin(), latencies0.end(), 0.0);
    auto sum1 = std::accumulate(latencies1.begin(), latencies1.end(), 0.0);

    BOOST_TEST_MESSAGE("Average WebSocket Message Propagation Loop Latency(Send): " << sum0 / TryCount
                                                                                    << " microseconds");
    BOOST_TEST_MESSAGE("Minimum WebSocket Message Propagation Latency(Send): " << latencies0.front()
                                                                               << " microseconds");
    BOOST_TEST_MESSAGE("Maximum WebSocket Message Propagation Latency(Send): " << latencies0.back() << " microseconds");
    BOOST_TEST_MESSAGE("Maximum WebSocket Message Propagation 95th Percentile Latency(Send): "
                       << latencies0[int(latencies0.size() * 0.95)] << " microseconds");
    BOOST_TEST_MESSAGE("Maximum WebSocket Message Propagation 99th Percentile Latency(Send): "
                       << latencies0[int(latencies0.size() * 0.99)] << " microseconds");

    BOOST_TEST_MESSAGE("Average WebSocket Message Propagation Latency(Receive): " << sum1 / TryCount
                                                                                  << " microseconds");
    BOOST_TEST_MESSAGE("Minimum WebSocket Message Propagation Latency(Receive): " << latencies1.front()
                                                                                  << " microseconds");
    BOOST_TEST_MESSAGE("Maximum WebSocket Message Propagation Latency(Receive): " << latencies1.back()
                                                                                  << " microseconds");
    BOOST_TEST_MESSAGE("Maximum WebSocket Message Propagation 95th Percentile Latency(Receive): "
                       << latencies1[int(latencies1.size() * 0.95)] << " microseconds");
    BOOST_TEST_MESSAGE("Maximum WebSocket Message Propagation 99th Percentile Latency(Receive): "
                       << latencies1[int(latencies1.size() * 0.99)] << " microseconds");
}

BOOST_AUTO_TEST_CASE(EndToEndTradingLoopLatency)
{
    std::vector<long long> latencies;

    for (int i = 0; i < TryCount; i++)
    {
        TradeSystem tradeSystem;

        PlaceOrderDesc orderDesc;
        orderDesc.Symbol    = "ETH-PERPETUAL";
        orderDesc.Amount    = 1.0;
        orderDesc.Direction = OrderDirection::Buy;
        orderDesc.Type      = OrderType::Limit;
        orderDesc.Price     = 800.0;

        std::string orderId;

        auto start    = high_resolution_clock::now();
        bool success  = tradeSystem.PlaceOrder(orderDesc, &orderId);     
        auto response = tradeSystem.GetOrderBook(orderDesc.Symbol, OrderBookDepth::_10000);
        auto end      = high_resolution_clock::now();

        BOOST_CHECK(success);
        if (success)
        {
            tradeSystem.CancelOrder(orderId);
        }

        auto duration = duration_cast<microseconds>(end - start).count();
        auto ns       = duration_cast<nanoseconds>(end - start).count();
        BOOST_TEST_MESSAGE("End-to-End Trading Loop Latency: " << duration << " microseconds (" << ns
                                                               << " nanoseconds)");

        latencies.push_back(duration);

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    std::sort(latencies.begin(), latencies.end());

    auto sum = std::accumulate(latencies.begin(), latencies.end(), 0.0);
    BOOST_TEST_MESSAGE("Average End-to-End Trading Loop Latency: " << sum / TryCount << " microseconds");
    BOOST_TEST_MESSAGE("Minimum End-to-End Trading Loop Latency: " << latencies.front() << " microseconds");
    BOOST_TEST_MESSAGE("Maximum End-to-End Trading Loop Latency: " << latencies.back() << " microseconds");
    BOOST_TEST_MESSAGE("Maximum End-to-End Trading Loop 95th Percentile Latency: "
                       << latencies[int(latencies.size() * 0.95)] << " microseconds");
    BOOST_TEST_MESSAGE("Maximum End-to-End Trading Loop 99th Percentile Latency: "
                       << latencies[int(latencies.size() * 0.99)] << " microseconds");
}

BOOST_AUTO_TEST_SUITE_END()