#include "cli.hpp"
#include <order-management/trade-system.hpp>
#include <order-management/order-desc.hpp>

namespace om
{
    static void DisplayOrderMenu(const PlaceOrderDesc& orderDesc)
    {
        std::cout << "=== Place Order ===\n";

        std::cout << "1. Enter symbol: (Current: '" << (orderDesc.Symbol.empty() ? NONE_STRING : orderDesc.Symbol)
                  << "')*\n";

        std::cout << "2. Enter amount: (Current: '"
                  << (orderDesc.Amount.has_value() ? std::to_string(*orderDesc.Amount) : NONE_STRING) << "')*\n";

        std::cout << "3. Enter direction: (Current: '"
                  << (orderDesc.Direction.has_value() ? magic_enum::enum_name(*orderDesc.Direction) : NONE_STRING)
                  << "')*\n";

        std::cout << "4. Enter type: (Current: '"
                  << (orderDesc.Type.has_value() ? magic_enum::enum_name(*orderDesc.Type) : NONE_STRING) << "')\n";

        std::cout << "5. Enter label: (Current: '" << orderDesc.Label << "')\n";

        std::cout << "6. Enter price: (Current: '"
                  << (orderDesc.Price.has_value() ? std::to_string(*orderDesc.Price) : NONE_STRING) << "')*\n";

        std::cout << "7. Enter reduce only: (Current: '"
                  << (orderDesc.ReduceOnly.has_value() ? std::to_string(*orderDesc.ReduceOnly) : NONE_STRING) << "')\n";

        std::cout << "8. Time in force: (Current: '"
                  << (orderDesc.TimeInForce.has_value() ? magic_enum::enum_name(*orderDesc.TimeInForce) : NONE_STRING)
                  << "')\n";

        std::cout << "9. Post only: (Current: '"
                  << (orderDesc.PostOnly.has_value() ? std::to_string(*orderDesc.PostOnly) : NONE_STRING) << "')\n";

        std::cout << "10. Reject post only: (Current: '"
                  << (orderDesc.RejectPostOnly.has_value() ? std::to_string(*orderDesc.RejectPostOnly) : NONE_STRING)
                  << "')\n";

        std::cout << "11. Trigger price: (Current: '"
                  << (orderDesc.TriggerPrice.has_value() ? std::to_string(*orderDesc.TriggerPrice) : NONE_STRING)
                  << "')\n";

        std::cout << "12. Trigger offset: (Current: '"
                  << (orderDesc.TriggerOffset.has_value() ? std::to_string(*orderDesc.TriggerOffset) : NONE_STRING)
                  << "')\n";

        std::cout << "13. Trigger: (Current: '"
                  << (orderDesc.Trigger.has_value() ? magic_enum::enum_name(*orderDesc.Trigger) : NONE_STRING)
                  << "')\n";

        std::cout << "14. Place order\n";

        std::cout << "15. Cancel\n";
    }

    [[nodiscard]] static std::optional<PlaceOrderDesc> ReadOrderDesc()
    {
        std::optional<PlaceOrderDesc> orderDesc = PlaceOrderDesc{};

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
                std::cout << "Enter symbol: ";
                std::getline(std::cin, orderDesc->Symbol);
                break;
            }

            case 2:
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

            case 3:
            {
                std::cout << "Enter direction (1=BUY, 2=SELL): ";
                int direction;
                if (ReadInputSafe(input, direction))
                {
                    switch (direction)
                    {
                    case 1:
                        orderDesc->Direction = OrderDirection::Buy;
                        break;
                    case 2:
                        orderDesc->Direction = OrderDirection::Sell;
                        break;
                    default:
                        orderDesc->Direction.reset();
                        break;
                    }
                }
                else
                {
                    orderDesc->Direction.reset();
                }
                break;
            }

            case 4:
            {
                std::cout << "Enter type (1=Limit, 2=StopLimit, 3=TakeLimit, 4=Market, 5=StopMarket, 6=TakeMarket, "
                             "7=MarketLimit, 8=TrailStop): ";
                int type;
                if (ReadInputSafe(input, type))
                {
                    switch (type)
                    {
                    case 1:
                        orderDesc->Type = OrderType::Limit;
                        break;
                    case 2:
                        orderDesc->Type = OrderType::StopLimit;
                        break;
                    case 3:
                        orderDesc->Type = OrderType::TakeLimit;
                        break;
                    case 4:
                        orderDesc->Type = OrderType::Market;
                        break;
                    case 5:
                        orderDesc->Type = OrderType::StopMarket;
                        break;
                    case 6:
                        orderDesc->Type = OrderType::TakeMarket;
                        break;
                    case 7:
                        orderDesc->Type = OrderType::MarketLimit;
                        break;
                    case 8:
                        orderDesc->Type = OrderType::TrailStop;
                        break;
                    default:
                        orderDesc->Type.reset();
                        break;
                    }
                }
                else
                {
                    orderDesc->Type.reset();
                }
                break;
            }

            case 5:
            {
                std::cout << "Enter label: ";
                std::getline(std::cin, orderDesc->Label);
                break;
            }

            case 6:
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

            case 7:
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

            case 8:
            {
                std::cout
                    << "Enter time in force (1=GoodTilCancelled, 2=GoodTilDay, 3=FillOrKill, 4=ImmediateOrCancel): ";
                int tif;
                if (ReadInputSafe(input, tif))
                {
                    switch (tif)
                    {
                    case 1:
                        orderDesc->TimeInForce = OrderRemainEffect::GoodTilCancelled;
                        break;
                    case 2:
                        orderDesc->TimeInForce = OrderRemainEffect::GoodTilDay;
                        break;
                    case 3:
                        orderDesc->TimeInForce = OrderRemainEffect::FillOrKill;
                        break;
                    case 4:
                        orderDesc->TimeInForce = OrderRemainEffect::ImmediateOrCancel;
                        break;
                    default:
                        orderDesc->TimeInForce.reset();
                        break;
                    }
                }
                else
                {
                    orderDesc->TimeInForce.reset();
                }
                break;
            }

            case 9:
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

            case 10:
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

            case 11:
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

            case 12:
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

            case 13:
            {
                std::cout << "Enter trigger (1=IndexPrice, 2=MarkPrice, 3=LastPrice): ";
                int trigger;
                if (ReadInputSafe(input, trigger))
                {
                    switch (trigger)
                    {
                    case 1:
                        orderDesc->Trigger = OrderPriceTrigger::IndexPrice;
                        break;
                    case 2:
                        orderDesc->Trigger = OrderPriceTrigger::MarkPrice;
                        break;
                    case 3:
                        orderDesc->Trigger = OrderPriceTrigger::LastPrice;
                        break;
                    default:
                        orderDesc->Trigger.reset();
                        break;
                    }
                }
                else
                {
                    orderDesc->Trigger.reset();
                }
                break;
            }

            case 14:
            {
                if (orderDesc->Symbol.empty() || !orderDesc->Amount.has_value() || !orderDesc->Direction.has_value() ||
                    !orderDesc->Price.has_value())
                {
                    std::cout << "Order is incomplete. Please fill in all required fields.\n";
                }
                else
                {
                    exit = true;
                }
                break;
            }

            case 15:
            {
                orderDesc.reset();
                exit = true;
                break;
            }
            }
        }

        return orderDesc;
    }

    void CLITradeSystem::PlaceOrder()
    {
        auto orderDesc = ReadOrderDesc();
        if (!orderDesc)
        {
            return;
        }

        std::string orderId;
        if (!m_TradeSystem.PlaceOrder(*orderDesc, &orderId))
        {
            spdlog::apply_all([&](auto logger) { logger->error("Failed to place order."); });
        }
        else
        {
            spdlog::apply_all([&](auto logger) { logger->info("Order placed successfully. Order ID: {}", orderId); });
        }
    }
} // namespace om