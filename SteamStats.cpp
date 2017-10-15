//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
// NOTICE: This file contains references to the Steamworks API. See the included
// LICENSE file for details and restrictions on using this file.
#include "ags2client/IAGS2Client.h"
#include "SteamStats.h"
#include "steam/steam_api.h"
using namespace AGSteam::Plugin;

namespace AGSteam
{
    namespace Plugin
    {
        struct SteamStats_Statics
        {
        public:
            static SteamStats STATS;
        };
    }
}

SteamStats SteamStats_Statics::STATS;

SteamStats& SteamStats::GetSteamStats() noexcept
{
    return SteamStats_Statics::STATS;
}

int SteamStats::GetIntStat(char const *name) const noexcept
{
	if (!AGS2Client::GetClient()->IsInitialized()) return 0;
	int32 i = 0;
	SteamUserStats()->GetStat(name, &i);
	return static_cast<int>(i);
}

float SteamStats::GetFloatStat(char const *name) const noexcept
{
	if (!AGS2Client::GetClient()->IsInitialized()) return 0.0f;
	float f = 0.0f;
	SteamUserStats()->GetStat(name, &f);
	return f;
}

float SteamStats::GetAverageRateStat(char const *name) const noexcept
{
	return GetFloatStat(name); // average rates are calculated as float values
}

bool SteamStats::SetIntStat(char const *name, int value) const noexcept
{
	if (!AGS2Client::GetClient()->IsInitialized()) return false;
	SteamUserStats()->SetStat(name, static_cast<int32>(value));
	int result = SteamUserStats()->StoreStats();
	SteamAPI_RunCallbacks();
	return (result != 0);
}

bool SteamStats::SetFloatStat(char const *name, float value) const noexcept
{
	if (!AGS2Client::GetClient()->IsInitialized()) return false;
	SteamUserStats()->SetStat(name, value);
	int result = SteamUserStats()->StoreStats();
	SteamAPI_RunCallbacks();
	return (result != 0);
}

bool SteamStats::UpdateAverageRateStat(char const *name, float numerator, float denominator) const noexcept
{
	if (!AGS2Client::GetClient()->IsInitialized()) return false;
	SteamUserStats()->UpdateAvgRateStat(name, numerator, denominator);
	int result = SteamUserStats()->StoreStats();
	SteamAPI_RunCallbacks();
	return (result != 0);
}
