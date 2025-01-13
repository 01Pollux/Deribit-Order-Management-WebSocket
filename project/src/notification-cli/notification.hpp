#pragma once

#include <concepts>
#include <string>
#include <iostream>
#include <thread>

namespace om
{
    class NotificationSystem;

    class CLINotificationSystem
    {
    public:
        CLINotificationSystem(NotificationSystem& notificationSystem);

    public:
        void Run();

    private:
        void Subscribe();
        void Unsubscribe();

    private:
        NotificationSystem& m_NotificationSystem;
    };
} // namespace om