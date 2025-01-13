#include <order-management/notification-system.hpp>

#include "notification.hpp"

int main()
{
    om::NotificationSystem notificationSystem;

    om::CLINotificationSystem cliNotificatrionSystem(notificationSystem);
    cliNotificatrionSystem.Run();

    return 0;
}
