//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2014 MonkeyMoto Productions, Inc.
//
// NOTICE: THIS FILE IS NOT OPEN SOURCE, AND SHOULD NEVER LEAVE THE PROPERTIES OF MONKEYMOTO PRODUCTIONS, INC.
// ("MMP") WITHOUT PRIOR EXPRESS WRITTEN PERMISSION INCLUDED AS AN ADDENDUM BELOW, ONLY BY AUTHORIZED
// PERSONNEL ACTING ON BEHALF OF MMP. IF YOU HAVE NOT BEEN SO AUTHORIZED AND HAVE OBTAINED THIS FILE, YOU ARE
// UNDER LEGAL OBLIGATION TO DELETE ALL COPIES OF IT AND ALL DERIVATIVE WORKS IMMEDIATELY. MMP AND AFFILIATED
// PARTIES SHALL NOT BE HELD LIABLE FOR ANY UNAUTHORIZED USAGE OR DISTRIBUTION OF THIS FILE, INTERNALLY OR BY
// ANY THIRD-PARTY. BY ACCESSING THIS FILE YOU ARE AGREEING TO THE TERMS AND CONDITIONS OF THIS NOTICE, AND
// ALL APPLICABLE ADDENDUMS, AND MAY BE HELD LEGALLY RESPONSIBLE FOR UNAUTHORIZED USAGE.
//
// ADDENDUM: MMP RESERVES ALL FIRST-PARTY RIGHTS TO THIS FILE AND ITS CONTENTS, INCLUDING, BUT NOT LIMITED TO
// THE RIGHT TO AUTHORIZE THIRD-PARTY USAGE OF THIS FILE, AND THE RIGHT TO REVOKE SUCH AUTHORIZATION FROM ANY
// THIRD-PARTY. AUTHORIZATION MAY BE GRANTED OR REVOKED AT ANY TIME AND FOR ANY REASON BY MMP. UPON NOTICE OF
// REVOCATION OF RIGHTS, THIRD-PARTIES MUST COMPLY IMMEDIATELY WITH THE REVISED TERMS GIVEN BY MMP. ANY
// ADDENDUMS OR CHANGES TO THIS ADDENDUM OR THE PRIOR NOTICE SHALL BE HELD NULL AND VOID BY MMP, PURSUANT TO
// THE RESTRICTIONS STATED WITHIN.
//
// ADDENDUM: IN AGREEMENT WITH WADJET EYE GAMES ("WEG"), THE FOLLOWING PRIVILEGES ARE GRANTED, BEGINNING ON
// 16 FEBRUARY 2013. AUTHORIZED PERSONNEL OF WEG ARE HEREBY AUTHORIZED BY MONKEYMOTO PRODUCTIONS, INC. TO
// ACCESS AND MODIFY THE CONTENTS OF THIS FILE, PURSUANT TO THE PRIOR RESTRICTIONS, AND THE TERMS AND
// CONDITIONS OF EXISTING NON-DISCLOSURE AGREEMENTS BETWEEN VALVE CORPORATION ("VALVE") AND MMP, AND VALVE AND
// WEG. LIMITED AUTHORIZATION IS ALSO GRANTED TO AUTHORIZED WEG PERSONNEL TO REDISTRIBUTE THIS FILE, PURSUANT
// TO THE FOLLOWING RESTRICTIONS:
//
//     THE ABOVE NOTICE AND ALL EXISTING ADDENDUMS MUST REMAIN UNCHANGED AT THE BEGINNING OF THIS C++ FILE.
//
//     WEG ACCEPTS FULL RESPONSIBILITY AND LIABILITY FOR ANY USAGE OF THIS FILE BY THIRD-PARTIES IT
//     AUTHORIZES, PURSUANT TO THEIR OWN TERMS AND RESTRICTIONS, WHICH MUST NOT PLACE ANY FURTHER LIABILITY
//     UPON MMP.
//
// ADDENDUM: ALL PARTIES ARE GRANTED NON-EXCLUSIVE LICENSE TO REDISTRIBUTE THE COMPILED FORM OF THIS C++ FILE
// FOR THE PURPOSES OF STATIC OR DYNAMIC CODE LINKING, SUCH AS IN THE FORM OF A WINDOWS DYNAMIC-LINK LIBRARY
// ("DLL") FILE. THIS ADDENDUM DOES NOT APPLY TO THE NON-COMPILED SOURCE OF THIS C++ FILE.
//
// ADDENDUM: ADDITIONAL RESTRICTIONS AND DEFINITION OF TERMS MAY BE INCLUDED IN A SEPARATE FILE NAMED
// "LICENSE" ("THE LICENSE FILE"), WITH NO TRAILING FILE EXTENSION. THE LICENSE FILE WILL BE WRITTEN IN PLAIN-
// TEXT, IN THE ENGLISH LANGUAGE, WITH A UTF-8 CHARACTER ENCODING. THE LICENSE FILE AND THIS ADDENDUM MAY BE
// AMENDED AS NECESSARY BY ANY AUTHORIZED PERSONS, WHILE UNAUTHORIZED CHANGES WILL BE NULL AND VOID.
//
#include "agsplugin.h"
#include "SteamLeaderboards.h"

SteamLeaderboard *SteamLeaderboards = NULL;
extern IAGSEngine *engine;
extern int const Steam_Initialized();

SteamLeaderboard::SteamLeaderboard() : CurrentLeaderboard(NULL), LeaderboardEntriesCount(0),
                                         CallResultFindLeaderboard(), CallResultUploadScore(),
                                         CallResultDownloadScore()
{
  for (int i = 0; i < 10; ++i)
  {
    LeaderboardEntries[i].m_steamIDUser.Clear();
  }
}

SteamLeaderboard::~SteamLeaderboard()
{
}

void SteamLeaderboard::FindLeaderboard(char const *leaderboardName)
{
  if (leaderboardName == NULL) return;
  CurrentLeaderboard = NULL;
  CallResultFindLeaderboard.Set(SteamUserStats()->FindLeaderboard(leaderboardName), this, &SteamLeaderboard::OnFindLeaderboard);
}

void SteamLeaderboard::OnFindLeaderboard(LeaderboardFindResult_t *callback, bool IOFailure)
{
  // see if there was an error
  if ((!callback->m_bLeaderboardFound) || (IOFailure))
  {
    OutputDebugString("Leaderboard could not be found\n");
    return;
  }
  CurrentLeaderboard = callback->m_hSteamLeaderboard;
}

bool SteamLeaderboard::UploadScore(int score)
{
  if (CurrentLeaderboard == NULL) return false;
  CallResultUploadScore.Set(SteamUserStats()->UploadLeaderboardScore(CurrentLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, static_cast<int32>(score), NULL, 0), this, &SteamLeaderboard::OnUploadScore);
  return true;
}

void SteamLeaderboard::OnUploadScore(LeaderboardScoreUploaded_t *callback, bool IOFailure)
{
  if ((!callback->m_bSuccess) || (IOFailure))
  {
    OutputDebugString("Score could not be uploaded to Steam\n");
  }
}

bool SteamLeaderboard::DownloadScores(AGSteamScoresRequestType type)
{
  if (CurrentLeaderboard == NULL) return false;
  CallResultDownloadScore.Set(SteamUserStats()->DownloadLeaderboardEntries(CurrentLeaderboard, static_cast<ELeaderboardDataRequest>(type), -4, 5), this, &SteamLeaderboard::OnDownloadScore);
  return true;
}

void SteamLeaderboard::OnDownloadScore(LeaderboardScoresDownloaded_t *callback, bool IOFailure)
{
  if (IOFailure) return;
  LeaderboardEntriesCount = min(callback->m_cEntryCount, 10);
  int i = 0;
  for ( ; i < LeaderboardEntriesCount; ++i)
  {
    SteamUserStats()->GetDownloadedLeaderboardEntry(callback->m_hSteamLeaderboardEntries, i, &LeaderboardEntries[i], NULL, 0);
  }
  for ( ; i < 10; ++i)
  {
    LeaderboardEntries[i].m_steamIDUser.Clear();
  }
}

char const* SteamLeaderboard::GetCurrentLeaderboardName()
{
  if ((!Steam_Initialized()) || (CurrentLeaderboard == NULL)) return "";
  return SteamUserStats()->GetLeaderboardName(CurrentLeaderboard);
}

char const* SteamLeaderboard::GetLeaderName(int index)
{
  if ((!Steam_Initialized()) || (index < 0) || (index > LeaderboardEntriesCount) || (!LeaderboardEntries[index].m_steamIDUser.IsValid())) return "";
  return SteamFriends()->GetFriendPersonaName(LeaderboardEntries[index].m_steamIDUser);
}

int SteamLeaderboard::GetLeaderScore(int index)
{
  if ((!Steam_Initialized()) || (index < 0) || (index > LeaderboardEntriesCount) || (!LeaderboardEntries[index].m_steamIDUser.IsValid())) return 0;
  return static_cast<int>(LeaderboardEntries[index].m_nScore);
}

AGSteamScoresRequestType MapAGSteamScoresRequestToNative(int raw)
{
  switch (raw)
  {
  case 0:
    return eAGSteamScoresRequestGlobal;
  case 1:
    return eAGSteamScoresRequestAroundUser;
  case 2:
    return eAGSteamScoresRequestFriends;
  default:
    return static_cast<AGSteamScoresRequestType>(0);
  }
}

int MapAGSteamScoresRequestToAGS(AGSteamScoresRequestType type)
{
  switch (type)
  {
  case eAGSteamScoresRequestAroundUser:
    return 1;
  case eAGSteamScoresRequestFriends:
    return 2;
  case eAGSteamScoresRequestGlobal:
  default:
    return 0;
  }
}

char const* SteamLeaderboard_GetCurrentLeadboardName()
{
  char const *leaderboardName = SteamLeaderboards->GetCurrentLeaderboardName();
  return engine->CreateScriptString(leaderboardName == NULL ? "" : leaderboardName);
}

void SteamLeaderboard_FindLeaderboard(char const *leaderboardName)
{
  SteamLeaderboards->FindLeaderboard(leaderboardName);
}

int SteamLeaderboard_UploadScore(int score)
{
  return SteamLeaderboards->UploadScore(score);
}

int SteamLeaderboard_DownloadScores(int rawType)
{
  return SteamLeaderboards->DownloadScores(MapAGSteamScoresRequestToNative(rawType));
}

char const* SteamLeaderboard_GetLeaderName(int index)
{
  char const *leaderName = SteamLeaderboards->GetLeaderName(index);
  return engine->CreateScriptString(leaderName == NULL ? "" : leaderName);
}

int SteamLeaderboard_GetLeaderScore(int index)
{
  return SteamLeaderboards->GetLeaderScore(index);
}

int SteamLeaderboard_GetLeaderCount()
{
  return SteamLeaderboards->LeaderboardEntriesCount;
}
