#include "cli.hpp"
#include <order-management/trade-system.hpp>
#include <order-management/position.hpp>

namespace om
{
    static void DisplayPositionMenu(const GetPositionsDesc& positionDesc)
    {
        std::cout << "=== Position ===\n";
        std::cout << "1. Set currency: (Current: '" << positionDesc.Currency << "')\n";
        std::cout << "2. Set kind: (Current: '"
                  << (positionDesc.Kind.has_value() ? magic_enum::enum_name(*positionDesc.Kind) : NONE_STRING)
                  << "')\n";
        std::cout << "3. Set sub account id: (Current: '"
                  << (positionDesc.SubAccountId.has_value() ? std::to_string(*positionDesc.SubAccountId) : NONE_STRING)
                  << "')\n";
        std::cout << "4. View positions\n";
        std::cout << "5. Exit\n";
    }

    static std::optional<GetPositionsDesc> ReadPositionDesc()
    {
        std::optional<GetPositionsDesc> positionDesc = GetPositionsDesc{};

        std::string inputStr;
        bool        exit = false;
        while (!exit)
        {
            DisplayPositionMenu(*positionDesc);
            int input;
            if (!ReadInputSafe(inputStr, input))
            {
                continue;
            }
            switch (input)
            {
            case 1:
                std::cout << "Enter currency: ";
                std::getline(std::cin, positionDesc->Currency);
                break;
            case 2:
                std::cout << "Enter kind (0=Future, 1=Option, 2=Spot, 3=FutureCombo, 4=OptionCombo): ";
                if (ReadInputSafe(inputStr, input))
                {
                    positionDesc->Kind = static_cast<PositionKind>(input);
                }
                else
                {
                    positionDesc->Kind.reset();
                }
                break;
            case 3:
                std::cout << "Enter sub account id: ";
                if (ReadInputSafe(inputStr, input))
                {
                    positionDesc->SubAccountId = input;
                }
                else
                {
                    positionDesc->SubAccountId.reset();
                }
                break;
            case 4:
                return positionDesc;
            case 5:
                exit = true;
                positionDesc.reset();
                break;
            }
        }
        return positionDesc;
    }

    static void PrintPosition(const PositionDesc& position)
    {
        std::cout << "=== Position ===\n";
        std::cout << "Average price: " << position.AveragePrice << "\n";
        std::cout << "Delta: " << position.Delta << "\n";
        std::cout << "Direction: " << magic_enum::enum_name(position.Direction) << "\n";
        std::cout << "Floating profit loss: " << position.FloatingProfitLoss << "\n";
        std::cout << "Realized profit loss: " << position.RealizedProfitLoss << "\n";
        std::cout << "Total profit loss: " << position.TotalProfitLoss << "\n";
        std::cout << "Size: " << position.Size << "\n";
    }

    //

    void CLITradeSystem::ViewPosition()
    {
        std::string symbol;
        std::cout << "Enter symbol: ";
        std::getline(std::cin, symbol);

        auto position = m_TradeSystem.GetPosition(symbol);
        if (position)
        {
            PrintPosition(*position);
        }
        else
        {
            spdlog::apply_all([&](auto logger) { logger->error("Failed to get positions"); });
        }
    }

    void CLITradeSystem::ViewPositions()
    {
        auto positionDesc = ReadPositionDesc();
        if (!positionDesc)
        {
            return;
        }

        auto positions = m_TradeSystem.GetPositions(*positionDesc);
        if (positions)
        {
            std::cout << "=== Positions ===\n";
            for (auto& position : *positions)
            {
                PrintPosition(position);
                std::cout << "========================\n";
            }
        }
        else
        {
            spdlog::apply_all([&](auto logger) { logger->error("Failed to get positions"); });
        }
    }
} // namespace om