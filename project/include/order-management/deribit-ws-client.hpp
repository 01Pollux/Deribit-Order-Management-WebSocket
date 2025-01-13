#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio.hpp>

#include "core.hpp"
#include "json-map.hpp"

namespace om
{
    class DeribitWSClient
    {
    private:
        using io_context_type  = boost::asio::io_context;
        using ssl_context_type = boost::asio::ssl::context;

        using tcp_type            = boost::asio::ip::tcp;
        using tcp_socket_type     = tcp_type::socket;
        using ssl_tcp_socket_type = boost::asio::ssl::stream<tcp_socket_type>;
        using websocket_type      = boost::beast::websocket::stream<ssl_tcp_socket_type>;

    public:
        DeribitWSClient();

        DeribitWSClient(const DeribitWSClient&)            = delete;
        DeribitWSClient& operator=(const DeribitWSClient&) = delete;

        DeribitWSClient(DeribitWSClient&&) noexcept            = default;
        DeribitWSClient& operator=(DeribitWSClient&&) noexcept = default;

        ~DeribitWSClient();

    public:
        /// <summary>
        /// Close the websocket
        /// </summary>
        void Close() noexcept;

    public:
        /// <summary>
        /// Send a message to the server
        /// </summary>
        [[nodiscard]] JsonMap Send(const JsonMap& message);

        /// <summary>
        /// Send a message to the server
        /// </summary>
        void Post(const JsonMap& message);

        /// <summary>
        /// Receive a notification if we have one
        /// </summary>
        [[nodiscard]] JsonMap ReceiveNotification();

        /// <summary>
        /// Receive a notification if we have one
        /// If the websocket is closed, return an empty optional
        /// </summary>
        [[nodiscard]] std::optional<JsonMap> ReceiveNotificationOpt();

    private:
        static auto SerializeToJsonText(const JsonMap& message) -> std::string;
        static auto DeserializeFromJsonText(const std::string& message) -> JsonMap;

    private:
        io_context_type  m_IoContext;
        ssl_context_type m_SslContext;

        tcp_type::resolver m_Resolver;
        websocket_type     m_WebSocket;

        boost::beast::flat_buffer m_Buffer;
    };
} // namespace om