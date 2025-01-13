-- project config
configs = {
    -- enable perfomance monitor for benchmarking, will use tracy
    benchmarking = true,
    -- enable custom benchmarking for benchmarking, will use our implementation of benchmarking 'TimedBenchmark'
    use_custom_benchmarking = true,
    -- enable test mode, where we will disable benchmarking and manually measure time to output each test
    -- for more accurate results
    test_mode = false
}

-- we are using xmake to build our project
add_rules("mode.debug")

-- optionally, we can set the build mode to release
-- add_rules("mode.releasedbg")

--

-- add boost, sdplog, openssl, magic_enum as dependencies
add_requires("boost", {configs = {
    cmake = false
}})
add_requires("spdlog", {configs = {
    std_format = true,
    wchar = false,
    noexcept = true
}})
add_requires("openssl")
add_requires("magic_enum")
add_requires("tracy")

--

-- use C++23 standard
set_languages("c++23")

-- use clang-format to format our code
add_extrafiles("project/.clang-format")

-- set warning as error
set_warnings("all", "error")

--

if configs.test_mode then
    -- disable benchmarking
    configs.benchmarking = false
    configs.use_custom_benchmarking = false
end

-- create our library target
target("order-manager")
    -- set type
    set_kind("static")

    -- add packages used by our project
    add_packages("boost", "spdlog", "openssl", "magic_enum", "tracy", {public = true, inherit = true})

    -- add files
    add_files("project/src/order-management/**.cpp")
    add_headerfiles("project/src/order-management/**.hpp")
    add_headerfiles("project/include/order-management/**.hpp", {public = true})

    -- add include directories
    add_includedirs("project/include", {public = true})

    -- add defines
    if is_mode("debug") then
        add_defines("OM_DEBUG", {public = true, inherit = true})
    end

    add_packages("tracy", {public = true, inherit = true})

    if configs.benchmarking then
        -- enable tracy profiler
        add_defines("TRACY_ENABLE", {public = true, inherit = true})
    end

    if configs.use_custom_benchmarking then
        -- enable custom benchmarking
        add_defines("OM_CUSTOM_BENCHMARKING", {public = true, inherit = true})
    end

    add_defines("WIN32_LEAN_AND_MEAN", {public = true, inherit = true})
    add_defines("NOMINMAX", {public = true, inherit = true})

-- create our trade system cli
target("trade-system-cli")
    -- set type
    set_kind("binary")

    -- add dependency to order-manager
    add_deps("order-manager")

    -- add files
    add_files("project/src/trade-system-cli/**.cpp")
    add_headerfiles("project/src/trade-system-cli/**.hpp")

-- create our notification cli
target("notification-cli")
    -- set type
    set_kind("binary")

    -- add dependency to order-manager
    add_deps("order-manager")

    -- add files
    add_files("project/src/notification-cli/**.cpp")
    add_headerfiles("project/src/notification-cli/**.hpp")

    -- create our tests for bonus section
target("order-management-tests")
    -- set type
    set_kind("binary")

    -- add dependency to order-manager
    add_deps("order-manager")

    -- add files
    add_files("project/tests/order-management/**.cpp")
    add_headerfiles("project/tests/order-management/**.hpp")
