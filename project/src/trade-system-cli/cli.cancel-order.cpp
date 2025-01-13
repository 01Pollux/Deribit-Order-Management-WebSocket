#include "cli.hpp"
#include <order-management/trade-system.hpp>

namespace om
{
    void CLITradeSystem::CancelOrder()
    {
        std::cout << "Enter the order ID: ";
        std::string orderId;
        std::getline(std::cin, orderId);

        if (m_TradeSystem.CancelOrder(orderId))
        {
            std::cout << "Order has been cancelled.\n";
        }
        else
        {
            std::cout << "Failed to cancel the order.\n";
        }
    }
} // namespace om