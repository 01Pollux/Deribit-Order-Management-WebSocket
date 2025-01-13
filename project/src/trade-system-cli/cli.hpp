#pragma once

#include <concepts>
#include <string>
#include <iostream>
#include <charconv>

namespace om
{
    class TradeSystem;

    enum class CLICommand
    {
        Unknown,
        PlaceOrder,
        CancelOrder,
        ModifyOrder,
        ViewOrderbook,
        ViewPosition,
        ViewPositions,
        Exit
    };

    class CLITradeSystem
    {
    public:
        CLITradeSystem(TradeSystem& tradeSystem);

    public:
        void Run();

    private:
        /// <summary>
        /// Print the menu to the console and read the user's choice.
        /// </summary>
        [[nodiscard]] CLICommand ReadCommand();

    private:
        void PlaceOrder();

        void CancelOrder();

        void ModifyOrder();

        void ViewOrderbook();

        void ViewPosition();

        void ViewPositions();

    private:
        TradeSystem& m_TradeSystem;
    };

    template<typename Ty>
        requires std::integral<Ty> || std::floating_point<Ty>
    inline bool ReadInputSafe(std::string& input, Ty& value)
    {
        std::getline(std::cin, input);
        return std::from_chars(input.data(), input.data() + input.size(), value).ec == std::errc();
    }
} // namespace om