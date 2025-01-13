#include <order-management/notification-system.hpp>
#include "mappers/subscription-mapper.hpp"

#include <order-management/benchmarking.hpp>

namespace om
{
    NotificationSystem::NotificationSystem() : m_AccessTokenHandler(m_DeribitWSClient)
    {
    }

    NotificationSystem::~NotificationSystem()
    {
        if (m_ThreadIsActive)
        {
            m_Thread.request_stop();
            m_DeribitWSClient.Close();
            m_Thread.join();
        }
    }

    //

    bool NotificationSystem::Subscribe(const std::string& symbol)
    {
        ZONE_BENCHMARK("NotificationSystem Subscribe");

        if (!m_AccessTokenHandler.TryToAuthenticate())
        {
            return false;
        }

        InitializeNotificationThreadOnce();

        auto request = SubscriptionMapper::MapToSubscribeRequest(symbol);
        m_DeribitWSClient.Post(request);

        return true;
    }

    bool NotificationSystem::Unsubscribe(const std::string& symbol)
    {
        ZONE_BENCHMARK("NotificationSystem Unsubscribe");

        if (!m_ThreadIsActive || !m_AccessTokenHandler.TryToAuthenticate())
        {
            return false;
        }

        auto request = SubscriptionMapper::MapToUnsubscribeRequest(symbol);
        m_DeribitWSClient.Post(request);

        return true;
    }

    void NotificationSystem::SetMessageHandler(MessageHandler handler)
    {
        std::scoped_lock lock(m_MessageHandlerMutex);
        m_MessageHandler = handler;
    }

    //

    void NotificationSystem::InitializeNotificationThreadOnce()
    {
        if (m_ThreadIsActive)
        {
            return;
        }

        m_Thread         = std::jthread(&NotificationSystem::NotificationThread, this);
        m_ThreadIsActive = true;
    }

    void NotificationSystem::NotificationThread()
    {
        auto stopToken = m_Thread.get_stop_token();
        while (!stopToken.stop_requested())
        {
            PollNotifications();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void NotificationSystem::PollNotifications()
    {
        auto message = m_DeribitWSClient.ReceiveNotificationOpt();
        if (!message)
        {
            return;
        }

        std::scoped_lock lock(m_MessageHandlerMutex);
        if (m_MessageHandler)
        {
            m_MessageHandler(*message);
        }
    }
} // namespace om