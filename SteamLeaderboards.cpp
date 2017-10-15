//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
// NOTICE: This file contains references to the Steamworks API. See the included
// LICENSE file for details and restrictions on using this file.
#include <algorithm>
#include <vector>
#include "ags2client/IAGS2Client.h"
#include "SteamLeaderboards.h"
#include "steam/steam_api.h"
#include "ags2client/Cpp11Fix.h"
using namespace AGSteam::Plugin;

struct LeaderboardListener
{
private:
    friend SteamLeaderboards_Statics;

#ifdef AGS2CLIENT_HAS_CPP11
	LeaderboardListener() = default;
#else
	LeaderboardListener() {};
#endif // AGS2CLIENT_HAS_CPP11

public:
    static LeaderboardListener& GetLeaderboardListener() noexcept;

#define LEADERBOARD_LISTENER_CALLRESULT(thisclass, func, param, var) CCallResult<thisclass, param> var; void func(param *pParam, bool bIOFailure)
	LEADERBOARD_LISTENER_CALLRESULT(LeaderboardListener, OnFindLeaderboard, LeaderboardFindResult_t, CallResultFindLeaderboard);
	LEADERBOARD_LISTENER_CALLRESULT(LeaderboardListener, OnUploadScore, LeaderboardScoreUploaded_t, CallResultUploadScore);
	LEADERBOARD_LISTENER_CALLRESULT(LeaderboardListener, OnDownloadScore, LeaderboardScoresDownloaded_t, CallResultDownloadScore);
#undef LEADERBOARD_LISTENER_CALLRESULT
};

namespace AGSteam
{
    namespace Plugin
    {
        struct SteamLeaderboards_Statics
        {
        public:
            static LeaderboardListener LISTENER;
            static SteamLeaderboards LEADERBOARDS;
        };
    }
}

LeaderboardListener SteamLeaderboards_Statics::LISTENER;
SteamLeaderboards SteamLeaderboards_Statics::LEADERBOARDS;

LeaderboardListener& LeaderboardListener::GetLeaderboardListener() noexcept
{
    return SteamLeaderboards_Statics::LISTENER;
}

static struct
{
	bool HasLeaderboard;
	SteamLeaderboard_t CurrentLeaderboard;
	std::vector<LeaderboardEntry_t> Entries;
	int Limit;
	ELeaderboardDataRequest Type;
} leaderboard;

SteamLeaderboards& SteamLeaderboards::GetSteamLeaderboards() noexcept
{
    return SteamLeaderboards_Statics::LEADERBOARDS;
}

void SteamLeaderboards::RequestLeaderboard(char const *leaderboardName, AGS2Client::LeaderboardScore::Type type, int limit) const noexcept
{
	if (leaderboardName == nullptr) return;
	leaderboard.HasLeaderboard = false;
	leaderboard.Entries.clear();
	leaderboard.Limit = limit;
	leaderboard.Type = static_cast<ELeaderboardDataRequest>(type);
	LeaderboardListener &listener = LeaderboardListener::GetLeaderboardListener();
	listener.CallResultFindLeaderboard.Set(SteamUserStats()->FindLeaderboard(leaderboardName), &listener, &LeaderboardListener::OnFindLeaderboard);
}

extern "C" void SteamLeaderboards_FindLeaderboard(char const *leaderboardName)
{
    SteamLeaderboards::GetSteamLeaderboards().RequestLeaderboard(nullptr, AGS2Client::LeaderboardScore::AroundUser, 10);
}

void LeaderboardListener::OnUploadScore(LeaderboardScoreUploaded_t *callback, bool IOFailure)
{
}

void LeaderboardListener::OnFindLeaderboard(LeaderboardFindResult_t *callback, bool IOFailure)
{
	// see if there was an error
	if ((!callback->m_bLeaderboardFound) || (IOFailure)) return;
	leaderboard.HasLeaderboard = true;
	leaderboard.CurrentLeaderboard = callback->m_hSteamLeaderboard;
	int rangeStart = 1;
	int rangeEnd = std::min(SteamUserStats()->GetLeaderboardEntryCount(leaderboard.CurrentLeaderboard), leaderboard.Limit);
	if (leaderboard.Type == k_ELeaderboardDataRequestGlobalAroundUser)
	{
		rangeStart = -(rangeEnd / 2) + !(rangeEnd % 2);
		rangeEnd = -rangeStart + 1;
	}
	CallResultDownloadScore.Set(SteamUserStats()->DownloadLeaderboardEntries(leaderboard.CurrentLeaderboard, static_cast<ELeaderboardDataRequest>(0), rangeStart, rangeEnd),
		&LeaderboardListener::GetLeaderboardListener(), &LeaderboardListener::OnDownloadScore);
}

void LeaderboardListener::OnDownloadScore(LeaderboardScoresDownloaded_t *callback, bool IOFailure)
{
	if (IOFailure) return;
	leaderboard.Entries.reserve(callback->m_cEntryCount);
	LeaderboardEntry_t entry;
	for (int i = 0; i < callback->m_cEntryCount; ++i)
	{
		SteamUserStats()->GetDownloadedLeaderboardEntry(callback->m_hSteamLeaderboardEntries, i, &entry, nullptr, 0);
		leaderboard.Entries.push_back(entry);
	}
}

bool SteamLeaderboard_HasValidLeaderboardInfo(int *index) noexcept
{
	return ((AGS2Client::GetClient()->IsInitialized()) && (leaderboard.HasLeaderboard) &&
		((index == nullptr) || (((*index) >= 0) && ((*index) < static_cast<int>(leaderboard.Entries.size())) && (leaderboard.Entries[*index].m_steamIDUser.IsValid()))));
}

bool SteamLeaderboards::UploadScore(int score) const noexcept
{
	if (!SteamLeaderboard_HasValidLeaderboardInfo(nullptr)) return false;
	LeaderboardListener &listener = LeaderboardListener::GetLeaderboardListener();
	listener.CallResultUploadScore.Set(SteamUserStats()->UploadLeaderboardScore(leaderboard.CurrentLeaderboard,
		k_ELeaderboardUploadScoreMethodKeepBest, static_cast<int32>(score), nullptr, 0), &listener, &LeaderboardListener::OnUploadScore);
	return true;
}

char const* SteamLeaderboards::GetCurrentLeaderboardName() const noexcept
{
	if (!SteamLeaderboard_HasValidLeaderboardInfo(nullptr)) return nullptr;
	return SteamUserStats()->GetLeaderboardName(leaderboard.CurrentLeaderboard);
}

char const* SteamLeaderboards::GetLeaderName(int index) const noexcept
{
	if (!SteamLeaderboard_HasValidLeaderboardInfo(&index)) return nullptr;
	return SteamFriends()->GetFriendPersonaName(leaderboard.Entries[index].m_steamIDUser);
}

int SteamLeaderboards::GetLeaderScore(int index) const noexcept
{
	if (!SteamLeaderboard_HasValidLeaderboardInfo(&index)) return 0;
	return static_cast<int>(leaderboard.Entries[index].m_nScore);
}

int SteamLeaderboards::GetLeaderCount() const noexcept
{
	return leaderboard.Entries.size();
}
