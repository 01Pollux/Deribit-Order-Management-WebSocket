#pragma once

#include "core.hpp"
#include <order-management/json-map.hpp>

namespace om
{
    class DeribitWSClient;

    class AccessTokenHandler
    {
    public:
        AccessTokenHandler(DeribitWSClient& deribitClient);

    public:
        /// <summary>
        /// Try to authenticate with the Deribit API
        /// If the access token is expired, it will try to refresh it
        /// </summary>
        [[nodiscard]] bool TryToAuthenticate();

    private:
        [[nodiscard]] bool CreateAccessToken();
        [[nodiscard]] bool RefreshAccessToken();

        void FetchAccessTokenResponse(const JsonMap& response);

    protected:
        DeribitWSClient& m_DeribitWSClient;

        std::string     m_AccessToken;
        std::string     m_RefreshAccessToken;
        SystemTimePoint m_AccessTokenExpireAt;
    };
} // namespace om