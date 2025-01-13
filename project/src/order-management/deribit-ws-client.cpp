#include <order-management/deribit-ws-client.hpp>
#include <order-management/config.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <boost/asio/connect.hpp>

#include <order-management/benchmarking.hpp>

namespace om
{
    namespace beast     = boost::beast;
    namespace http      = beast::http;
    namespace websocket = beast::websocket;
    namespace net       = boost::asio;
    namespace ssl       = boost::asio::ssl;

    DeribitWSClient::DeribitWSClient() :
        // Enable concurrency hint with '1' thread
        m_IoContext(),
        // Create an SSL context TLS v1.2
        m_SslContext(ssl::context::tlsv12_client),
        // Create a resolver
        m_Resolver(m_IoContext),
        // Create a WebSocket stream
        m_WebSocket(m_IoContext, m_SslContext)
    {
        try
        {
            ZONE_BENCHMARK("WebSocket Initialization");

            // Load the default certificate file
            m_SslContext.set_default_verify_paths();

            // Set the SSL context verification mode
            m_SslContext.set_verify_mode(ssl::verify_peer);

            // Resolve the host
            auto const results = m_Resolver.resolve(config::DeribitHost, config::DeribitPort);

            net::connect(beast::get_lowest_layer(m_WebSocket), results);

            // Set SNI hostname
            if (!SSL_set_tlsext_host_name(m_WebSocket.next_layer().native_handle(), config::DeribitHost))
            {
                throw beast::system_error(
                    beast::error_code(static_cast<int>(ERR_get_error()), net::error::get_ssl_category()),
                    "Failed to set SNI Hostname");
            }

            beast::error_code ec;
            m_WebSocket.next_layer().handshake(ssl::stream_base::client, ec);

            m_WebSocket.set_option(websocket::stream_base::decorator(
                [](websocket::request_type& req) { req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING); }));

            // Perform WebSocket handshake
            m_WebSocket.handshake(config::DeribitHost, config::DeribitWSApiEndPoint);
        }
        catch (const std::exception& e)
        {
            spdlog::apply_all([&](auto logger) { logger->error("Failed to initialize WebSocket: {}", e.what()); });
            Close();
            throw;
        }
    }

    DeribitWSClient::~DeribitWSClient()
    {
        Close();
    }

    //

    void DeribitWSClient::Close() noexcept
    {
        if (!m_WebSocket.is_open())
        {
            return;
        }

        ZONE_BENCHMARK("WebSocket Close");

        beast::error_code ec;
        // Close the WebSocket
        m_WebSocket.close(websocket::close_code::normal, ec);

        // Shutdown the SSL layer
        m_WebSocket.next_layer().shutdown(ec);
    }

    JsonMap DeribitWSClient::Send(const JsonMap& message)
    {
        ZONE_BENCHMARK("WebSocket Send Message");

        Post(message);
        return ReceiveNotification();
    }

    void DeribitWSClient::Post(const JsonMap& message)
    {
        ZONE_BENCHMARK("WebSocket Post Message");

        // Serialize the message to JSON text
        auto json = SerializeToJsonText(message);

        // Send the message
        m_WebSocket.write(net::buffer(json));
    }

    JsonMap DeribitWSClient::ReceiveNotification()
    {
        ZONE_BENCHMARK("WebSocket Receive Message");

        // Clear the buffer
        m_Buffer.consume(m_Buffer.size());

        // Read the response
        m_WebSocket.read(m_Buffer);

        return DeserializeFromJsonText(beast::buffers_to_string(m_Buffer.data()));
    }

    std::optional<JsonMap> DeribitWSClient::ReceiveNotificationOpt()
    {
        ZONE_BENCHMARK("WebSocket Receive Message Optional");

        // Clear the buffer
        m_Buffer.consume(m_Buffer.size());

        beast::error_code ec;
        // Read the response
        m_WebSocket.read(m_Buffer, ec);

        std::optional<JsonMap> message;
        if (!ec)
        {
            // Deserialize the response
            message.emplace(DeserializeFromJsonText(beast::buffers_to_string(m_Buffer.data())));
        }
        return message;
    }

    //

    std::string DeribitWSClient::SerializeToJsonText(const JsonMap& message)
    {
        ZONE_BENCHMARK("Serialize To JSON Text");

        std::ostringstream stream;
        boost::property_tree::write_json(stream, message);
        return stream.str();
    }

    JsonMap DeribitWSClient::DeserializeFromJsonText(const std::string& message)
    {
        ZONE_BENCHMARK("Deserialize From JSON Text");

        std::istringstream          stream(message);
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(stream, pt);
        return pt;
    }
} // namespace om