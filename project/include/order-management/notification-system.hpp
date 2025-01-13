#pragma once

#include <thread>

#include "deribit-ws-client.hpp"
#include "access-token-handler.hpp"

namespace om
{
    class NotificationSystem
    {
    public:
        using MessageHandler = std::function<void(const JsonMap&)>;

    public:
        NotificationSystem();

        NotificationSystem(const NotificationSystem&)            = delete;
        NotificationSystem& operator=(const NotificationSystem&) = delete;

        NotificationSystem(NotificationSystem&&)            = default;
        NotificationSystem& operator=(NotificationSystem&&) = default;

        ~NotificationSystem();

    public:
        /// <summary>
        /// Subscribe to a symbol
        /// </summary>
        [[nodiscard]] bool Subscribe(const std::string& symbol);

        /// <summary>
        /// Unsubscribe from a symbol
        /// </summary>
        [[nodiscard]] bool Unsubscribe(const std::string& symbol);

        /// <summary>
        /// Set the message handler
        /// </summary>
        [[nodiscard]] void SetMessageHandler(MessageHandler handler);

    private:
        /// <summary>
        /// Initialize the notification thread once
        /// </summary>
        void InitializeNotificationThreadOnce();

        /// <summary>
        /// The notification thread which will receive notifications
        /// </summary>
        void NotificationThread();

        /// <summary>
        /// Poll notifications from websocket
        /// </summary>
        void PollNotifications();

    private:
        DeribitWSClient    m_DeribitWSClient;
        AccessTokenHandler m_AccessTokenHandler;

        bool         m_ThreadIsActive = false;
        std::jthread m_Thread;

        std::mutex     m_MessageHandlerMutex;
        MessageHandler m_MessageHandler;
    };
} // namespace om