#include "cli.hpp"
#include <order-management/trade-system.hpp>

namespace om
{
    static void PrintOrderBook(const OrderBook& orderBook)
    {
        std::cout << "=== Order Book ===\n";

        std::cout << "State: " << magic_enum::enum_name(orderBook.CurrentState) << '\n';

        std::cout << "Asks:\n";
        for (const auto& ask : orderBook.Asks)
        {
            std::cout << "Price: " << ask.Price << ", Amount: " << ask.Amount << '\n';
        }

        std::cout << "Bids:\n";
        for (const auto& bid : orderBook.Bids)
        {
            std::cout << "Price: " << bid.Price << ", Amount: " << bid.Amount << '\n';
        }

        std::cout << "Best Ask: Price: " << orderBook.BestAsk.Price << ", Amount: " << orderBook.BestAsk.Amount << '\n';
        std::cout << "Best Bid: Price: " << orderBook.BestBid.Price << ", Amount: " << orderBook.BestBid.Amount << '\n';

        std::cout << "=== End Order Book ===\n";
    }

    void CLITradeSystem::ViewOrderbook()
    {
        std::string symbol;
        std::string depthInput;

        std::cout << "Enter symbol: ";
        std::getline(std::cin, symbol);

        OrderBookDepth depth = OrderBookDepth::Invalid;
        while (depth == OrderBookDepth::Invalid)
        {
            std::cout << "Enter depth ('', 1, 5, 10, 20, 50, 100, 1000, 10000): ";
            std::getline(std::cin, depthInput);

            if (depthInput.empty())
            {
                depth = OrderBookDepth::_1;
            }
            else
            {
                // parse from string to depth enum magic_enum
                depth = magic_enum::enum_cast<OrderBookDepth>("_" + depthInput).value_or(OrderBookDepth::Invalid);
            }
        }

        auto orderBook = m_TradeSystem.GetOrderBook(symbol, depth);
        if (!orderBook)
        {
            spdlog::apply_all([&](auto logger) { logger->error("Failed to get order book."); });
        }
        else
        {
            PrintOrderBook(*orderBook);
        }
    }
} // namespace om