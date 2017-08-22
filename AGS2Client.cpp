//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
#include "AGSteamPlugin.h"
#include "SteamAchievements.h"
#include "SteamLeaderboards.h"
#include "SteamStats.h"
using namespace AGSteam::Plugin;

#ifndef NDEBUG
#include <fstream>
#endif // NDEBUG

namespace AGS2Client
{
	IAGS2Client* GetClient() noexcept
	{
#ifndef NDEBUG
        static std::ofstream ofstream{ "agsteam_debug.log", std::ios::app };
        if (ofstream.is_open())
        {
            ofstream << "AGSteam::GetClient called, returning address '0x" << &AGSteamPlugin::GetAGSteamPlugin() << "'" << std::endl;
            ofstream.flush();
            ofstream.close();
        }
#endif // NDEBUG
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
