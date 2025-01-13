#include <order-management/trade-system.hpp>
#include "cli.hpp"

int main()
{
    om::TradeSystem tradeSystem;

    om::CLITradeSystem cli(tradeSystem);
    cli.Run();

    return 0;
}
