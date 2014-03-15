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
#include "SteamStats.h"

extern int const Steam_Initialized();
SteamStat *SteamStats = NULL;

SteamStat::SteamStat() : AppID(0), Initialized(false), CallbackUserStatsReceived(this, &SteamStat::OnUserStatsReceived),
                         CallbackUserStatsStored(this, &SteamStat::OnUserStatsStored),
                         CallResultGlobalStatsReceived()
{
	if (!Steam_Initialized()) return;
	AppID = SteamUtils()->GetAppID();
	RequestStats();
  CallResultGlobalStatsReceived.Set((SteamUserStats() == NULL ? 0 : SteamUserStats()->RequestGlobalStats(0)), this, &SteamStat::OnGlobalStatsReceived);
}

SteamStat::~SteamStat()
{
}

bool SteamStat::RequestStats()
{
	if ((!Steam_Initialized()) || (SteamUserStats() == NULL) || (SteamUser() == NULL)) return false; // Steam not loaded
	if (!SteamUser()->BLoggedOn()) return false; // not logged on
	return SteamUserStats()->RequestCurrentStats();
}

bool SteamStat::StoreStats()
{
	if ((!Steam_Initialized()) || (!Initialized)) return false;
	return SteamUserStats()->StoreStats();
}

void SteamStat::OnUserStatsReceived(UserStatsReceived_t *pCallback)
{
	if (pCallback->m_nGameID != AppID) return; // callback is for another game's stats, ignore it
	if (pCallback->m_eResult == k_EResultOK)
	{
		OutputDebugString("Received stats and achievements from Steam\n");
		Initialized = true;
	}
	else
	{
		char buffer[128];
		_snprintf(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
		OutputDebugString(buffer);
	}
}

void SteamStat::OnGlobalStatsReceived(GlobalStatsReceived_t *pCallResult, bool IOFailure)
{
  if (pCallResult->m_nGameID != AppID) return;
  if (pCallResult->m_eResult == k_EResultOK)
  {
    OutputDebugString("Received global stats from Steam\n");
  }
  else
  {
    char buffer[128];
    _snprintf(buffer, 128, "RequestGlobalStats - failed, %d\n", pCallResult->m_eResult);
    OutputDebugString(buffer);
  }
}

void SteamStat::OnUserStatsStored(UserStatsStored_t *pCallback)
{
	if (pCallback->m_nGameID != AppID) return; // callback is for another game's stats, ignore them
	if (pCallback->m_eResult == k_EResultOK) OutputDebugString("StoreStats - success\n");
	else if (pCallback->m_eResult == k_EResultInvalidParam)
	{
		// One or more stats we set broke a constraint. They've been reverted,
		// and we should re-interate the values now to keep in sync.
		OutputDebugString("StoreStats - some failed to validate\n");
		// Fake a callback here so that we reload the values
		UserStatsReceived_t callback;
		callback.m_eResult = k_EResultOK;
		callback.m_nGameID = AppID;
		OnUserStatsReceived(&callback);
	}
	else
	{
		char buffer[128];
		_snprintf(buffer, 128, "StoreStats - failed, %d\n", pCallback->m_eResult);
		OutputDebugString(buffer);
	}
}

int SteamStat::GetIntStat(char const *name)
{
  if ((!Steam_Initialized()) || (!Initialized)) return 0;
  int32 i = 0;
  SteamUserStats()->GetStat(name, &i);
  return static_cast<int>(i);
}

int SteamStat::GetGlobalIntStat(char const *name)
{
  if ((!Steam_Initialized()) || (!Initialized)) return 0;
  int64 i = 0;
  SteamUserStats()->GetGlobalStat(name, &i);
  return static_cast<int>(i);
}

float SteamStat::GetFloatStat(char const *name)
{
  if ((!Steam_Initialized()) || (!Initialized)) return 0.0f;
  float f = 0.0f;
	SteamUserStats()->GetStat(name, &f);
	return f;
}

float SteamStat::GetGlobalFloatStat(char const *name)
{
  if ((!Steam_Initialized()) || (!Initialized)) return 0;
  double d = 0.0f;
  SteamUserStats()->GetGlobalStat(name, &d);
  return static_cast<float>(d);
}

float SteamStat::GetAvgRateStat(char const *name)
{
  return GetFloatStat(name); // average rates are calculated as float values
}

bool SteamStat::SetIntStat(char const *name, int value)
{
  if ((!Steam_Initialized()) || (!Initialized)) return false;
  SteamUserStats()->SetStat(name, static_cast<int32>(value));
	return StoreStats();
}

bool SteamStat::SetFloatStat(char const *name, float value)
{
  if ((!Steam_Initialized()) || (!Initialized)) return false;
  SteamUserStats()->SetStat(name, value);
	return StoreStats();
}

bool SteamStat::UpdateAvgRateStat(char const *name, float numerator, float denominator)
{
  if ((!Steam_Initialized()) || (!Initialized)) return false;
  SteamUserStats()->UpdateAvgRateStat(name, numerator, denominator);
  GetFloatStat(name); // the API examples always call this for average rates, not sure if this is a necessary step here
	return StoreStats();
}

// AGS helper methods
int SteamStat_GetIntStat(char const *name)
{
	return SteamStats->GetIntStat(name);
}

int SteamStat_GetGlobalIntStat(char const *name)
{
  return SteamStats->GetIntStat(name);
}

float SteamStat_GetFloatStat(char const *name)
{
	return SteamStats->GetFloatStat(name);
}

float SteamStat_GetGlobalFloatStat(char const *name)
{
  return SteamStats->GetGlobalFloatStat(name);
}

float SteamStat_GetAvgRateStat(char const *name)
{
	return SteamStats->GetAvgRateStat(name);
}

int SteamStat_SetIntStat(char const *name, int value)
{
  return SteamStats->SetIntStat(name, value);
}

int SteamStat_SetFloatStat(const char *name, float value)
{
  return SteamStats->SetFloatStat(name, value);
}

int SteamStat_UpdateAvgRateStat(const char *name, float numerator, float denominator)
{
  return SteamStats->UpdateAvgRateStat(name, numerator, denominator);
}

void SteamStat_ResetStats()
{
	if (!Steam_Initialized()) return;
	SteamUserStats()->ResetAllStats(false);
}
