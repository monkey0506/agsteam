//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
// NOTICE: This file contains references to the Steamworks API. See the included
// LICENSE file for details and restrictions on using this file.
#include "ags2client/agsplugin.h"
#include "AGSteamPlugin.h"
#include "steam/steam_api.h"
using namespace AGSteam::Plugin;

#ifndef NDEBUG
#include <fstream>
#endif // DEBUG

static bool SteamInitialized = false;

struct UserStatsReceivedListener
{
public:
	static UserStatsReceivedListener& GetListener() noexcept
	{
		static UserStatsReceivedListener listener{};
		return listener;
	}

	STEAM_CALLBACK(UserStatsReceivedListener, OnUserStatsReceived, UserStatsReceived_t, CallbackUserStatsReceived);

private:
	UserStatsReceivedListener() noexcept : CallbackUserStatsReceived{ this, &UserStatsReceivedListener::OnUserStatsReceived }
	{
	}
};

void UserStatsReceivedListener::OnUserStatsReceived(UserStatsReceived_t *pCallback)
{
	if (pCallback->m_nGameID != SteamUtils()->GetAppID()) return;
	if (pCallback->m_eResult == k_EResultOK)
	{
		SteamInitialized = true; // a callback has been received, stat+achievement info is now accessible
	}
}

AGSteamPlugin& AGSteamPlugin::GetAGSteamPlugin() noexcept
{
	static AGSteamPlugin plugin{};
	return plugin;
}

#ifndef NDEBUG
void debug_log(std::ofstream &ofstream)
{
}

template<typename T>
void debug_log(std::ofstream &ofstream, T &&t)
{
    ofstream << std::forward<T>(t);
}

template<typename T, typename ...Args>
void debug_log(std::ofstream &ofstream, T &&t, Args &&...args)
{
    debug_log(ofstream, std::forward<T>(t));
    debug_log(ofstream, std::forward<Args>(args)...);
}
#endif // NDEBUG

template<typename ...Args>
void debug_log(Args ...args)
{
#ifndef NDEBUG
    static std::ofstream ofstream{};
    ofstream.open("agsteam_debug.log", std::ios::app);
    debug_log(ofstream, std::forward<Args>(args)...);
    ofstream << std::endl;
    ofstream.flush();
    ofstream.close();
#endif // NDEBUG
}

void AGSteamPlugin_Initialize() noexcept
{
	if (!SteamInitialized)
	{
        debug_log("Steam not initialized, calling SteamAPI_Init()");
		if (!SteamAPI_Init())
		{
            debug_log("SteamAPI_Init() failed. Is Steam running and logged in?");
			return;
		}
        debug_log("SteamAPI_Init() succeeded, creating UserStatsReceivedListener");
		//auto &listener = UserStatsReceivedListener::GetListener();
		UserStatsReceivedListener::GetListener(); // ensure that listener is created
        debug_log("UserStatsReceivedListener created, requesting current stats. SteamUserStats = '0x", SteamUserStats(), "'");
		SteamUserStats()->RequestCurrentStats();
        debug_log("User stats requested, AGSteamPlugin_Initialize() complete");
	}
    else
    {
        debug_log("Steam already initialized");
    }
}

bool AGSteamPlugin::IsInitialized() const noexcept
{
	// helper method for the plugin to ensure that the call to Steam is placed before any other Steam functions
	// this function also serves for the AGS property
	return SteamInitialized;
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
	return 3.2f;
}

char const* AGSteamPlugin::GetAGSPluginName() const noexcept
{
	return "AGSteam";
}

char const* AGSteamPlugin::GetAGSPluginDesc() const noexcept
{
	return "AGSteam: Steam API Plugin for AGS (C) 2011-2017 MonkeyMoto Productions, Inc.";
}

bool AGSteamPlugin::ClaimKeyPress(int data, int(*IsKeyPressed)(int)) const noexcept
{
	return (IsInitialized() && SteamUtils()->IsOverlayEnabled());
}

#ifndef NDEBUG
extern void SteamLeaderboards_FindLeaderboard(char const *leaderboardName);

void AGSteamPlugin::RegisterScriptFunctions(IAGSEngine *engine) const noexcept
{
    IAGS2Client::RegisterScriptFunctions(engine);
    static std::string findLeaderboard = std::string{ GetClientNameForScript() } + "::FindLeaderboard^1";
    engine->RegisterScriptFunction(findLeaderboard.c_str(), reinterpret_cast<void*>(SteamLeaderboards_FindLeaderboard));
}
#endif // NDEBUG
