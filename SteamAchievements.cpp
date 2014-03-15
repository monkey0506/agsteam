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
#include "SteamAchievements.h"

#ifndef WINDOWS_VERSION
extern int const Steam_Initialized();
#endif // WINDOWS_VERSION
SteamAchievement *SteamAchievements = NULL;

SteamAchievement::SteamAchievement() : AppID(0), Initialized(false),
                                       CallbackUserStatsReceived(this, &SteamAchievement::OnUserStatsReceived),
                                       CallbackUserStatsStored(this, &SteamAchievement::OnUserStatsStored),
                                       CallbackAchievementStored(this, &SteamAchievement::OnAchievementStored)
{
	if (!Steam_Initialized()) return;
	AppID = SteamUtils()->GetAppID();
	RequestStats();
}

SteamAchievement::~SteamAchievement()
{
}

bool SteamAchievement::ClearAchievement(char const *ID)
{
  if ((!Steam_Initialized()) || (!Initialized)) return false;
  SteamUserStats()->ClearAchievement(ID);
  return SteamUserStats()->StoreStats(); // confusingly named, this stores the stats and achievements
}

bool SteamAchievement::IsAchievementAchieved(char const *ID)
{
	if ((!Steam_Initialized()) || (!Initialized)) return false; // Steam not initialized, we can't do anything
	bool achieved = false;
	SteamUserStats()->GetAchievement(ID, &achieved);
	return achieved;
}

bool SteamAchievement::RequestStats() // helper to validate necessary info before the request
{
	if ((!Steam_Initialized()) || (SteamUserStats() == NULL) || (SteamUser() == NULL)) return false; // Steam not loaded
	if (!SteamUser()->BLoggedOn()) return false; // user not logged on
	return SteamUserStats()->RequestCurrentStats();
}

int SteamAchievement::SetAchievementAchieved(char const *ID)
{
	if ((!Steam_Initialized()) || (!Initialized)) return false; // Steam not initialized or haven't received call back from Steam yet, so we can't set achievements
	SteamUserStats()->SetAchievement(ID);
	return SteamUserStats()->StoreStats();
}

void SteamAchievement::OnUserStatsReceived(UserStatsReceived_t *pCallback)
{
	if (pCallback->m_nGameID != AppID) return;
	if (pCallback->m_eResult == k_EResultOK)
	{
		OutputDebugString("Received stats and achievements from Steam\n");
		Initialized = true; // a callback has been received, achievement info is now accessible
	}
	else
	{
		char buffer[128];
		_snprintf(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
		OutputDebugString(buffer);
	}
}

void SteamAchievement::OnUserStatsStored(UserStatsStored_t *pCallback)
{
	if (pCallback->m_nGameID != AppID) return; // callback is for another game's stats, ignore it
	if (pCallback->m_eResult == k_EResultOK) OutputDebugString("Stored stats for Steam\n");
	else
	{
		char buffer[128];
		_snprintf(buffer, 128, "StoreStats - failed, %d\n", pCallback->m_eResult);
		OutputDebugString(buffer);
	}
}

void SteamAchievement::OnAchievementStored(UserAchievementStored_t *pCallback)
{
	if (pCallback->m_nGameID == AppID) OutputDebugString("Stored Achievement for Steam\n"); // callback may be for another game's stats, if so, ignore it
}

// Helper methods because the AGS plugin API requires casting the function pointer to void*
int SteamAchievement_ClearAchievement(char const *ID)
{
  return SteamAchievements->ClearAchievement(ID);
}

int SteamAchievement_SetAchieved(char const *ID)
{
	return SteamAchievements->SetAchievementAchieved(ID);
}

int SteamAchievement_IsAchievementAchieved(char const *ID)
{
	return SteamAchievements->IsAchievementAchieved(ID);
}
