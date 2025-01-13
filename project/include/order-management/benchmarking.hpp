#pragma once

#include "core.hpp"
#include <tracy/Tracy.hpp>

namespace om
{
    /// <summary>
    /// Measures the time taken by a scope
    /// </summary>
    class TimedBenchmark
    {
    public:
        explicit TimedBenchmark(const char* name) : m_StartTimepoint(HighResClock::now()), m_Name(name)
        {
        }

        TimedBenchmark(const TimedBenchmark&)            = delete;
        TimedBenchmark& operator=(const TimedBenchmark&) = delete;

        TimedBenchmark(TimedBenchmark&&)            = delete;
        TimedBenchmark& operator=(TimedBenchmark&&) = delete;

        ~TimedBenchmark()
        {
            auto cur  = HighResClock::now();
            auto diff = cur - m_StartTimepoint;

            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
            auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count();

            spdlog::apply_all([&](const auto& logger)
                              { logger->info("Benchmark: {} took {}ms ({}ns)", m_Name, ms, ns); });
        }

    private:
        const char*      m_Name;
        HighResTimePoint m_StartTimepoint;
    };
} // namespace om

// May report incorrect results since it outputs the results to the console
#if OM_CUSTOM_BENCHMARKING
#define ZONE_BENCHMARK(name)                                                                                           \
    om::TimedBenchmark om_benchmark##__LINE__(name);                                                                   \
    ZoneScopedN(name)
#else
#define ZONE_BENCHMARK(name) ZoneScopedN(name)
#endif
