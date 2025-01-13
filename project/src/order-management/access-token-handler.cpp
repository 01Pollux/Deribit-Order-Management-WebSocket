#include <order-management/access-token-handler.hpp>
#include <order-management/deribit-ws-client.hpp>

#include "mappers/auth-mapper.hpp"
#include "mappers/common-json-map.hpp"

#include <order-management/benchmarking.hpp>

namespace om
{
    AccessTokenHandler::AccessTokenHandler(DeribitWSClient& deribitClient) : m_DeribitWSClient(deribitClient)
    {
    }

    //

    bool AccessTokenHandler::TryToAuthenticate()
    {
        if (!m_AccessToken.empty() && (SystemClock::now() < m_AccessTokenExpireAt))
        {
            return true;
        }

        if (!m_RefreshAccessToken.empty())
        {
            return CreateAccessToken();
        }
        else
        {
            return RefreshAccessToken();
        }
    }

    //

    bool AccessTokenHandler::CreateAccessToken()
    {
        ZONE_BENCHMARK("Refresh Access Token");

        auto request  = AuthMapper::CreateAuthRequest();
        auto response = m_DeribitWSClient.Send(request);
        if (CommonJsonMap::PrintOnError(response))
        {
            return false;
        }

        FetchAccessTokenResponse(response);

        return true;
    }

    bool AccessTokenHandler::RefreshAccessToken()
    {
        ZONE_BENCHMARK("Create Access Token");

        auto request  = AuthMapper::RefreshAuthRequest(m_RefreshAccessToken);
        auto response = m_DeribitWSClient.Send(request);
        if (CommonJsonMap::PrintOnError(response))
        {
            return false;
        }

        FetchAccessTokenResponse(response);

        return true;
    }

    //

    void AccessTokenHandler::FetchAccessTokenResponse(const JsonMap& response)
    {
        auto& result          = response.get_child("result");
        m_AccessToken         = result.get<std::string>("access_token");
        m_RefreshAccessToken  = result.get<std::string>("refresh_token");
        m_AccessTokenExpireAt = SystemClock::now() + std::chrono::seconds(result.get<int>("expires_in"));
    }
} // namespace om