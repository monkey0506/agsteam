//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
// NOTICE: This file contains references to the Steamworks API. See the included
// LICENSE file for details and restrictions on using this file.
#include "ags2client/IAGS2Client.h"
#include "SteamAchievements.h"
#include "steam/steam_api.h"
using namespace AGSteam::Plugin;

SteamAchievements& SteamAchievements::GetSteamAchievements() noexcept
{
	static SteamAchievements achievements;
	return achievements;
}

bool SteamAchievements::ResetAchievement(char const *ID) const noexcept
{
	if (!AGS2Client::GetClient()->IsInitialized()) return false;
	SteamUserStats()->ClearAchievement(ID);
	return SteamUserStats()->StoreStats();
}

bool SteamAchievements::IsAchievementAchieved(char const *ID) const noexcept
{
	if (!AGS2Client::GetClient()->IsInitialized()) return false; // Steam not initialized, we can't do anything
	bool achieved = false;
	SteamUserStats()->GetAchievement(ID, &achieved);
	return achieved;
}

bool SteamAchievements::SetAchievementAchieved(char const *ID) const noexcept
{
	if (!AGS2Client::GetClient()->IsInitialized()) return false; // Steam not initialized or haven't received call back from Steam yet, so we can't set achievements
	int result = SteamUserStats()->SetAchievement(ID);
	SteamUserStats()->StoreStats();
	SteamAPI_RunCallbacks();
	return (result != 0);
}
