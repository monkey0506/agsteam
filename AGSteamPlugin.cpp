//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
// NOTICE: This file contains references to the Steamworks API. See the included
// LICENSE file for details and restrictions on using this file.
#include <string>
#include "ags2client/agsplugin.h"
#include "AGSteamPlugin.h"
#include "steam/steam_api.h"
using namespace AGSteam::Plugin;

namespace AGSteam
{
    namespace Plugin
    {
        template<typename Listener, typename Event>
        struct SteamEventListener
        {
        protected:
            CCallback<Listener, Event> callback;

            virtual void OnEventReceived(Event *event) = 0;

            SteamEventListener() : callback(const_cast<Listener*>(static_cast<Listener const* const>(this)), &SteamEventListener::OnEventReceived)
            {
            }

        public:
            virtual ~SteamEventListener() = default;
        };

        struct UserStatsReceivedListener : public SteamEventListener<UserStatsReceivedListener, UserStatsReceived_t>
        {
        private:
            void OnEventReceived(UserStatsReceived_t *event) override;
        };

        struct GameOverlayActivatedListener : public SteamEventListener<GameOverlayActivatedListener, GameOverlayActivated_t>
        {
        private:
            void OnEventReceived(GameOverlayActivated_t *event) override;
        };

        struct AGSteamPlugin_Statics
        {
        public:
            static std::string FIND_LEADERBOARD;
            static GameOverlayActivatedListener GAME_OVERLAY_ACTIVATED_LISTENER;
            static bool GAME_OVERLAY_ACTIVE;
            static bool INITIALIZED;
            static AGSteamPlugin PLUGIN;
            static UserStatsReceivedListener USER_STATS_RECEIVED_LISTENER;
        };
    }
}

std::string AGSteamPlugin_Statics::FIND_LEADERBOARD;
GameOverlayActivatedListener AGSteamPlugin_Statics::GAME_OVERLAY_ACTIVATED_LISTENER;
bool AGSteamPlugin_Statics::GAME_OVERLAY_ACTIVE = false;
bool AGSteamPlugin_Statics::INITIALIZED = false;
AGSteamPlugin AGSteamPlugin_Statics::PLUGIN;
UserStatsReceivedListener AGSteamPlugin_Statics::USER_STATS_RECEIVED_LISTENER;

void UserStatsReceivedListener::OnEventReceived(UserStatsReceived_t *event)
{
    if (event->m_nGameID != SteamUtils()->GetAppID())
    {
        return;
    }
    if (event->m_eResult == k_EResultOK)
    {
        AGSteamPlugin_Statics::INITIALIZED = true; // a callback has been received, stat+achievement info is now accessible
    }
}

void GameOverlayActivatedListener::OnEventReceived(GameOverlayActivated_t *event)
{
    AGSteamPlugin_Statics::GAME_OVERLAY_ACTIVE = event->m_bActive;
}

AGSteamPlugin& AGSteamPlugin::GetAGSteamPlugin() noexcept
{
    return AGSteamPlugin_Statics::PLUGIN;
}

void AGSteamPlugin_Initialize() noexcept
{
	if (!AGSteamPlugin_Statics::INITIALIZED)
	{
		if (!SteamAPI_Init())
		{
			return;
		}
        (void)AGSteamPlugin_Statics::GAME_OVERLAY_ACTIVATED_LISTENER; // ensure that game overlay listener is created
        (void)AGSteamPlugin_Statics::USER_STATS_RECEIVED_LISTENER; // ensure that user stats listener is created
		SteamUserStats()->RequestCurrentStats();
	}
}

bool AGSteamPlugin::IsInitialized() const noexcept
{
	// helper method for the plugin to ensure that the call to Steam is placed before any other Steam functions
	// this function also serves for the AGS property
	return AGSteamPlugin_Statics::INITIALIZED;
}

void AGSteamPlugin::ResetStatsAndAchievements() const noexcept
{
	if (!IsInitialized()) return;
	SteamUserStats()->ResetAllStats(true);
}

char const* AGSteamPlugin::GetCurrentGameLanguage() const noexcept
{
	return (IsInitialized() ? SteamApps()->GetCurrentGameLanguage() : nullptr);
}

char const* AGSteamPlugin::GetUserName() const noexcept
{
	return (IsInitialized() ? SteamFriends()->GetPersonaName() : nullptr);
}

void AGSteamPlugin::Startup() const noexcept
{
	AGSteamPlugin_Initialize();
}

void AGSteamPlugin::Shutdown() const noexcept
{
    SteamAPI_Shutdown();
}

void AGSteamPlugin::Update() const noexcept
{
	SteamAPI_RunCallbacks();
}

float AGSteamPlugin::GetVersion() const noexcept
{
	return 3.4f;
}

char const* AGSteamPlugin::GetAGSPluginName() const noexcept
{
	return "AGSteam";
}

char const* AGSteamPlugin::GetAGSPluginDesc() const noexcept
{
	return "AGSteam: Steam API Plugin for AGS (C) 2011-2017 MonkeyMoto Productions, Inc.";
}

bool AGSteamPlugin::ClaimKeyPress(int data, int (*IsKeyPressed)(int)) const noexcept
{
	return AGSteamPlugin_Statics::GAME_OVERLAY_ACTIVE;
}

extern "C" void SteamLeaderboards_FindLeaderboard(char const *leaderboardName);

void AGSteamPlugin::RegisterScriptFunctions(IAGSEngine *engine) const noexcept
{
    std::string &findLeaderboard = AGSteamPlugin_Statics::FIND_LEADERBOARD;
    if (findLeaderboard.empty())
    {
        findLeaderboard = std::string(GetClientNameForScript()) + "::FindLeaderboard^1";
    }
    IAGS2Client::RegisterScriptFunctions(engine);
    engine->RegisterScriptFunction(findLeaderboard.c_str(), reinterpret_cast<void*>(SteamLeaderboards_FindLeaderboard));
}
