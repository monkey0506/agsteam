//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
#include "AGSteamPlugin.h"
#include "SteamAchievements.h"
#include "SteamLeaderboards.h"
#include "SteamStats.h"
using namespace AGSteam::Plugin;

namespace AGS2Client
{
	IAGS2Client* GetClient() noexcept
	{
		return &AGSteamPlugin::GetAGSteamPlugin();
	}

	IClientAchievements* GetClientAchievements() noexcept
	{
		return &SteamAchievements::GetSteamAchievements();
	}

	IClientLeaderboards* GetClientLeaderboards() noexcept
	{
		return &SteamLeaderboards::GetSteamLeaderboards();
	}

	IClientStats* GetClientStats() noexcept
	{
		return &SteamStats::GetSteamStats();
	}
}
