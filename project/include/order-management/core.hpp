#pragma once

#include <string>
#include <chrono>
#include <expected>
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>

namespace om
{
    inline constexpr const char* NONE_STRING           = "<NONE>";
    inline constexpr const char* INTERNAL_ERROR_STRING = "Internal Error";

    enum class OperationResult : uint8_t
    {
        UnknownError,

        Success,

        Unauthorized,
    };

    using SystemClock     = std::chrono::system_clock;
    using SystemTimePoint = std::chrono::time_point<SystemClock>;

    using HighResClock     = std::chrono::high_resolution_clock;
    using HighResDuration  = HighResClock::duration;
    using HighResTimePoint = std::chrono::time_point<HighResClock>;

    struct PlaceOrderDesc;
    struct ModifyOrderDesc;
} // namespace om