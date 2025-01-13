#include "cli.hpp"
#include <order-management/trade-system.hpp>
#include <order-management/order-desc.hpp>

namespace om
{
    static void DisplayOrderMenu(const ModifyOrderDesc& orderDesc)
    {
        std::cout << "=== Place Order ===\n";

        std::cout << "1. Enter amount: (Current: '"
                  << (orderDesc.Amount.has_value() ? std::to_string(*orderDesc.Amount) : NONE_STRING) << "')*\n";

        std::cout << "2. Enter price: (Current: '"
                  << (orderDesc.Price.has_value() ? std::to_string(*orderDesc.Price) : NONE_STRING) << "')*\n";

        std::cout << "3. Post only: (Current: '"
                  << (orderDesc.PostOnly.has_value() ? std::to_string(*orderDesc.PostOnly) : NONE_STRING) << "')\n";

        std::cout << "4. Enter reduce only: (Current: '"
                  << (orderDesc.ReduceOnly.has_value() ? std::to_string(*orderDesc.ReduceOnly) : NONE_STRING) << "')\n";

        std::cout << "5. Reject post only: (Current: '"
                  << (orderDesc.RejectPostOnly.has_value() ? std::to_string(*orderDesc.RejectPostOnly) : NONE_STRING)
                  << "')\n";

        std::cout << "6. Advanced: (Current: '"
                  << (orderDesc.Advanced.has_value() ? magic_enum::enum_name(*orderDesc.Advanced) : NONE_STRING)
                  << "')\n";

        std::cout << "7. Trigger price: (Current: '"
                  << (orderDesc.TriggerPrice.has_value() ? std::to_string(*orderDesc.TriggerPrice) : NONE_STRING)
                  << "')\n";

        std::cout << "8. Trigger offset: (Current: '"
                  << (orderDesc.TriggerOffset.has_value() ? std::to_string(*orderDesc.TriggerOffset) : NONE_STRING)
                  << "')\n";

        std::cout << "9. Modify order\n";

        std::cout << "10. Cancel\n";
    }

    [[nodiscard]] static std::optional<ModifyOrderDesc> ReadOrderDesc()
    {
        std::optional<ModifyOrderDesc> orderDesc = ModifyOrderDesc{};

        std::string input;

        bool exit = false;
        while (!exit)
        {
            DisplayOrderMenu(*orderDesc);

            //

            int choice;
            std::cout << "Enter your choice: ";
            ReadInputSafe(input, choice);

            //

            switch (choice)
            {
            case 1:
            {
                std::cout << "Enter amount: ";
                double amount;
                if (ReadInputSafe(input, amount))
                {
                    orderDesc->Amount = amount;
                }
                else
                {
                    orderDesc->Amount.reset();
                }
                break;
            }

            case 2:
            {
                std::cout << "Enter price: ";
                double price;
                if (ReadInputSafe(input, price))
                {
                    orderDesc->Price = price;
                }
                else
                {
                    orderDesc->Price.reset();
                }
                break;
            }

            case 3:
            {
                std::cout << "Enter post only (1=true, 0=false): ";
                int postOnly;
                if (ReadInputSafe(input, postOnly))
                {
                    orderDesc->PostOnly = postOnly;
                }
                else
                {
                    orderDesc->PostOnly.reset();
                }
                break;
            }

            case 4:
            {
                std::cout << "Enter reduce only (1=true, 0=false): ";
                int reduceOnly;
                if (ReadInputSafe(input, reduceOnly))
                {
                    orderDesc->ReduceOnly = reduceOnly;
                }
                else
                {
                    orderDesc->ReduceOnly.reset();
                }
                break;
            }

            case 5:
            {
                std::cout << "Enter reject post only (1=true, 0=false): ";
                int rejectPostOnly;
                if (ReadInputSafe(input, rejectPostOnly))
                {
                    orderDesc->RejectPostOnly = rejectPostOnly;
                }
                else
                {
                    orderDesc->RejectPostOnly.reset();
                }
                break;
            }

            case 6:
            {
                std::cout << "Enter advanced (1=USD, 2=IMPLV): ";
                int advanced;
                if (ReadInputSafe(input, advanced))
                {
                    switch (advanced)
                    {
                    case 1:
                        orderDesc->Advanced = OrderAdvancedOption::USD;
                        break;
                    case 2:
                        orderDesc->Advanced = OrderAdvancedOption::IMPLV;
                        break;
                    default:
                        orderDesc->Advanced.reset();
                        break;
                    }
                }
                else
                {
                    orderDesc->Advanced.reset();
                }
                break;
            }

            case 7:
            {
                std::cout << "Enter trigger price: ";
                double triggerPrice;
                if (ReadInputSafe(input, triggerPrice))
                {
                    orderDesc->TriggerPrice = triggerPrice;
                }
                else
                {
                    orderDesc->TriggerPrice.reset();
                }
                break;
            }

            case 8:
            {
                std::cout << "Enter trigger offset: ";
                double triggerOffset;
                if (ReadInputSafe(input, triggerOffset))
                {
                    orderDesc->TriggerOffset = triggerOffset;
                }
                else
                {
                    orderDesc->TriggerOffset.reset();
                }
                break;
            }

            case 9:
            {
                if (!orderDesc->Amount.has_value() || !orderDesc->Price.has_value())
                {
                    std::cout << "Order is incomplete. Please fill in all required fields.\n";
                }
                else
                {
                    exit = true;
                }
                break;
            }

            case 10:
            {
                orderDesc.reset();
                exit = true;
                break;
            }

            default:
            {
                std::cout << "Invalid command. Please try again.\n";
                break;
            }
            }
        }

        return orderDesc;
    }

    void CLITradeSystem::ModifyOrder()
    {
        auto orderDesc = ReadOrderDesc();
        if (orderDesc)
        {
            std::string orderId;
            std::cout << "Enter order ID: ";
            std::getline(std::cin, orderId);

            if (!m_TradeSystem.ModifyOrder(orderId, *orderDesc))
            {
                spdlog::apply_all([&](auto logger) { logger->error("Failed to modify order."); });
            }
            else
            {
                spdlog::apply_all([&](auto logger) { logger->info("Order modified successfully."); });
            }
        }
        else
        {
            spdlog::apply_all([&](auto logger) { logger->info("Order modification cancelled."); });
        }
    }
} // namespace om