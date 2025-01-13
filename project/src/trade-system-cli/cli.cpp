#include "cli.hpp"

namespace om
{
    CLITradeSystem::CLITradeSystem(TradeSystem& tradeSystem) : m_TradeSystem(tradeSystem)
    {
    }

    void CLITradeSystem::Run()
    {
        while (true)
        {
            auto command = ReadCommand();
            switch (command)
            {
            case CLICommand::PlaceOrder:
                PlaceOrder();
                break;

            case CLICommand::CancelOrder:
                CancelOrder();
                break;

            case CLICommand::ModifyOrder:
                ModifyOrder();
                break;

            case CLICommand::ViewOrderbook:
                ViewOrderbook();
                break;

            case CLICommand::ViewPosition:
                ViewPosition();
                break;

            case CLICommand::ViewPositions:
                ViewPositions();
                break;

            case CLICommand::Exit:
                return;

            default:
                std::unreachable();
            }
        }
    }

    //

    CLICommand CLITradeSystem::ReadCommand()
    {
        std::cout << "\n=== Trading System Console ===\n"
                  << "1. Place Order\n"
                  << "2. Cancel Order\n"
                  << "3. Modify Order\n"
                  << "4. View Orderbook\n"
                  << "5. View Position\n"
                  << "6. View Positions\n"
                  << "7. Exit\n"
                  << "==========================\n";

        CLICommand  command = CLICommand::Unknown;
        std::string input;
        do
        {
            std::cout << "Enter your choice: ";

            int choice = -1;
            ReadInputSafe(input, choice);

            switch (choice)
            {
            case 1:
                command = CLICommand::PlaceOrder;
                break;

            case 2:
                command = CLICommand::CancelOrder;
                break;

            case 3:
                command = CLICommand::ModifyOrder;
                break;

            case 4:
                command = CLICommand::ViewOrderbook;
                break;

            case 5:
                command = CLICommand::ViewPosition;
                break;

            case 6:
                command = CLICommand::ViewPositions;
                break;

            case 7:
                command = CLICommand::Exit;
                break;

            default:
                std::cout << "Invalid command. Please try again.\n";
                break;
            }
        } while (command == CLICommand::Unknown);

        return command;
    }
} // namespace om