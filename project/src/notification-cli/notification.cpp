#include <order-management/notification-system.hpp>
#include "notification.hpp"

#include <iostream>
#include <sstream>
#include <syncstream>

namespace om
{
    static void PrintMenu()
    {
        std::osyncstream os(std::cout);
        os << "=== Notification System ===\n";
        os << "1. Subscribe\n";
        os << "2. Unsubscribe\n";
        os << "3. Exit\n";
        os << "Enter choice: ";
    }

    CLINotificationSystem::CLINotificationSystem(NotificationSystem& notificationSystem) :
        m_NotificationSystem(notificationSystem)
    {
        m_NotificationSystem.SetMessageHandler([](auto& message)
                                               { std::cout << "Received message: " << ToString(message) << '\n'; });
    }

    void CLINotificationSystem::Run()
    {
        while (true)
        {
            PrintMenu();

            std::string input;
            std::getline(std::cin, input);

            if (input == "1")
            {
                Subscribe();
            }
            else if (input == "2")
            {
                Unsubscribe();
            }
            else if (input == "3")
            {
                return;
            }
            else
            {
                std::cout << "Invalid input\n";
            }
        }
    }

    void CLINotificationSystem::Subscribe()
    {
        std::cout << "Enter the symbol to subscribe to: ";

        std::string symbol;
        std::getline(std::cin, symbol);

        if (!m_NotificationSystem.Subscribe(symbol))
        {
            std::cout << "Failed to subscribe to symbol: " << symbol << '\n';
        }
        else
        {
            std::cout << "Subscribed to symbol: " << symbol << '\n';
        }
    }

    void CLINotificationSystem::Unsubscribe()
    {
        std::cout << "Enter the symbol to unsubscribe from: ";

        std::string symbol;
        std::getline(std::cin, symbol);

        if (!m_NotificationSystem.Unsubscribe(symbol))
        {
            std::cout << "Failed to unsubscribe from symbol: " << symbol << '\n';
        }
        else
        {
            std::cout << "Unsubscribed from symbol: " << symbol << '\n';
        }
    }
} // namespace om