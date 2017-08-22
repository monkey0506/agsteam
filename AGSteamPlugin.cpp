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

void AGSteamPlugin_Initialize() noexcept
{
	if (!SteamInitialized)
	{
		if (!SteamAPI_Init())
		{
			return;
		}
		//auto &listener = UserStatsReceivedListener::GetListener();
		UserStatsReceivedListener::GetListener(); // ensure that listener is created
		SteamUserStats()->RequestCurrentStats();
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
