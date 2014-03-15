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
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // WIN32_LEAN_AND_MEAN
#elif defined(__linux__) // !_WIN32
#define __stdcall
#endif // _WIN32, __linux__

#define THIS_IS_THE_PLUGIN
#include "agsplugin.h"
#include "steam/steam_api.h"
#include "SteamAchievements.h"
#include "SteamStats.h"
#include "SteamLeaderboards.h"

// ***** DESIGN TIME CALLS *******

#ifdef WINDOWS_VERSION

BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
  // this is the WINDOWS entry point into the DLL
  // everything we need occurs in the AGS entry points below
  return TRUE;
}

IAGSEditor *editor;

char const *ourScriptHeader =
    "#define AGSteam_VERSION 3.0\r\n"
    "\r\n"
    "enum AGSteamStatType\r\n"
    "{\r\n"
    "  eAGSteamStatInt = 0,\r\n"
    "  eAGSteamStatFloat = 1,\r\n"
    "  eAGSteamStatAverageRate = 2\r\n"
    "};\r\n"
    "\r\n"
    "enum AGSteamScoresRequestType\r\n"
    "{\r\n"
    "  eAGSteamScoresRequestGlobal = 0,\r\n"
    "  eAGSteamScoresRequestAroundUser = 1,\r\n"
    "  eAGSteamScoresRequestFriends = 2\r\n"
    "};\r\n"
    "\r\n"
    "managed struct AGSteam\r\n"
    "{\r\n"
    "  ///AGSteam: Returns whether the specified Steam achievement has been achieved\r\n"
    "  import static int IsAchievementAchieved(const string steamAchievementID);\r\n"
    "  ///AGSteam: Sets a Steam achievement as achieved\r\n"
    "  import static int SetAchievementAchieved(const string steamAchievementID);\r\n"
    "  ///AGSteam: Resets the specified Steam achievement, so it can be achieved again\r\n"
    "  import static int ResetAchievement(const string steamAchievementID);\r\n"
    "  ///AGSteam: Sets the value of a Steam INT stat\r\n"
    "  import static int SetIntStat(const string steamStatName, int value);\r\n"
    "  ///AGSteam: Sets the value of a Steam FLOAT stat\r\n"
    "  import static int SetFloatStat(const string steamStatName, float value);\r\n"
    "  ///AGSteam: Updates a Steam AVGRATE stat\r\n"
    "  import static int UpdateAverageRateStat(const string steamStatName, float numerator, float denominator);\r\n"
    "  ///AGSteam: Returns the value of a Steam INT stat\r\n"
    "  import static int GetIntStat(const string steamStatName);\r\n"
    "  ///AGSteam: Returns the value of a Steam FLOAT stat\r\n"
    "  import static float GetFloatStat(const string steamStatName);\r\n"
    "  ///AGSteam: Returns the value of a Steam AVGRATE stat\r\n"
    "  import static float GetAverageRateStat(const string steamStatName);\r\n"
    "  ///AGSteam: Resets all Steam stats to their default values\r\n"
    "  import static void ResetStats();\r\n"
    "  ///AGSteam: Resets all Steam stats and achievements\r\n"
    "  import static void ResetStatsAndAchievements();\r\n"
    "  ///AGSteam: Returns whether the Steam client is loaded and the user logged in\r\n"
    "  readonly import static attribute int Initialized;\r\n"
    "  ///AGSteam: Returns the value of a global Steam INT stat\r\n"
    "  import static int GetGlobalIntStat(const string steamStatName);\r\n"
    "  ///AGSteam: Returns the value of a global Steam FLOAT stat\r\n"
    "  import static float GetGlobalFloatStat(const string steamStatName);\r\n"
    "  ///AGSteam: Returns the name of the current leaderboard (call FindLeadboard first)\r\n"
    "  readonly import static attribute String CurrentLeaderboardName;\r\n"
    "  ///AGSteam: Requests to load the specified Steam leaderboard. This call is asynchronous and does not return the data immediately, check for results in repeatedly_execute.\r\n"
    "  import static void FindLeaderboard(const string leaderboardName);\r\n"
    "  ///AGSteam: Uploads the score to the current Steam leaderboard. Returns false if an error occurred.\r\n"
    "  import static int UploadScore(int score);\r\n"
    "  ///AGSteam: Downloads a list of ten scores from the current Steam leaderboard.\r\n"
    "  import static int DownloadScores(AGSteamScoresRequestType);\r\n"
    "  ///AGSteam: Returns the name associated with a downloaded score. Call DownloadScores first.\r\n"
    "  readonly import static attribute String LeaderboardNames[];\r\n"
    "  ///AGSteam: Returns a downloaded score. Call DownloadScores first.\r\n"
    "  readonly import static attribute int LeaderboardScores[];\r\n"
    "  ///AGSteam: Returns the number of downloaded scores (if any). Call DownloadScores first. Max is 10 scores.\r\n"
    "  readonly import static attribute int LeaderboardCount;\r\n"
    "  ///AGSteam: Returns the current game language as registered by the Steam client.\r\n"
    "  import static String GetCurrentGameLanguage();\r\n"
    "  ///AGSteam: Returns the Steam user's username.\r\n"
    "  import static String GetUserName();\r\n"
    "};\r\n"
    "\r\n";

LPCSTR AGS_GetPluginName()
{
    return "AGSteam";
}

int  AGS_EditorStartup(IAGSEditor *lpEditor)
{
    // User has checked the plugin to use it in their game

    // If it's an earlier version than what we need, abort.
    if (lpEditor->version < 1) return -1;

    editor = lpEditor;
    editor->RegisterScriptHeader(ourScriptHeader);

    // Return 0 to indicate success
    return 0;
}

void AGS_EditorShutdown()
{
    // User has un-checked the plugin from their game
    editor->UnregisterScriptHeader(ourScriptHeader);
}

void AGS_EditorProperties(HWND parent)
{
    // User has chosen to view the Properties of the plugin
    // We could load up an options dialog or something here instead
    MessageBox(parent, "AGSteam: Steam API Plugin for AGS (C) 2011-2014 MonkeyMoto Productions, Inc.", "About", MB_OK | MB_ICONINFORMATION);
}

int AGS_EditorSaveGame(char *buffer, int bufsize)
{
    // We don't want to save any persistent data
    return 0;
}

void AGS_EditorLoadGame(char *buffer, int bufsize)
{
    // Nothing to load for this plugin
}

#endif // WINDOWS_VERSION

// ******* END DESIGN TIME  *******


// ****** RUN TIME ********

extern SteamAchievement *SteamAchievements;
extern SteamStat *SteamStats;
extern SteamLeaderboard *SteamLeaderboards;

int const Steam_Initialized()
{
    // helper method for the plugin to ensure that the call to Steam is placed before any other Steam functions
    // this function also serves for the AGS property
    static bool steamInitialized = false;
    static bool donePluginInit = false;
    if (!steamInitialized)
    {
      steamInitialized = SteamAPI_Init();
      if (steamInitialized)
      {
        SteamAchievements = new SteamAchievement();
        SteamStats = new SteamStat();
        SteamLeaderboards = new SteamLeaderboard();
      }
    }
    return steamInitialized;
}

void Steam_ResetStatsAndAchievements()
{
    if (!Steam_Initialized()) return;
    SteamUserStats()->ResetAllStats(true);
}

IAGSEngine *engine;

char const* Steam_GetCurrentGameLanguage()
{
  return engine->CreateScriptString(SteamApps()->GetCurrentGameLanguage());
}

char const* Steam_GetUserName()
{
  return engine->CreateScriptString(SteamFriends()->GetPersonaName());
}

void AGS_EngineStartup(IAGSEngine *lpEngine)
{
    engine = lpEngine;

    if (engine->version < 17)
    {
        engine->AbortGame("Engine interface is too old, need newer version of AGS.");
    }

    engine->RegisterScriptFunction("AGSteam::IsAchievementAchieved^1", reinterpret_cast<void*>(SteamAchievement_IsAchievementAchieved));
    engine->RegisterScriptFunction("AGSteam::SetAchievementAchieved^1", reinterpret_cast<void*>(SteamAchievement_SetAchieved));
    engine->RegisterScriptFunction("AGSteam::ResetAchievement^1", reinterpret_cast<void*>(SteamAchievement_ClearAchievement));
    engine->RegisterScriptFunction("AGSteam::GetIntStat^1", reinterpret_cast<void*>(SteamStat_GetIntStat));
    engine->RegisterScriptFunction("AGSteam::GetFloatStat^1", reinterpret_cast<void*>(SteamStat_GetFloatStat));
    engine->RegisterScriptFunction("AGSteam::GetAverageRateStat^1", reinterpret_cast<void*>(SteamStat_GetAvgRateStat));
    engine->RegisterScriptFunction("AGSteam::SetIntStat^2", reinterpret_cast<void*>(SteamStat_SetIntStat));
    engine->RegisterScriptFunction("AGSteam::SetFloatStat^2", reinterpret_cast<void*>(SteamStat_SetFloatStat));
    engine->RegisterScriptFunction("AGSteam::UpdateAverageRateStat^3", reinterpret_cast<void*>(SteamStat_UpdateAvgRateStat));
    engine->RegisterScriptFunction("AGSteam::ResetStats^0", reinterpret_cast<void*>(SteamStat_ResetStats));
    engine->RegisterScriptFunction("AGSteam::ResetStatsAndAchievements^0", reinterpret_cast<void*>(Steam_ResetStatsAndAchievements));
    engine->RegisterScriptFunction("AGSteam::get_Initialized", reinterpret_cast<void*>(Steam_Initialized));
    engine->RegisterScriptFunction("AGSteam::GetGlobalIntStat^1", reinterpret_cast<void*>(SteamStat_GetGlobalIntStat));
    engine->RegisterScriptFunction("AGSteam::GetGlobalFloatStat^1", reinterpret_cast<void*>(SteamStat_GetGlobalFloatStat));
    engine->RegisterScriptFunction("AGSteam::get_CurrentLeaderboardName", reinterpret_cast<void*>(SteamLeaderboard_GetCurrentLeadboardName));
    engine->RegisterScriptFunction("AGSteam::FindLeaderboard^1", reinterpret_cast<void*>(SteamLeaderboard_FindLeaderboard));
    engine->RegisterScriptFunction("AGSteam::UploadScore^1", reinterpret_cast<void*>(SteamLeaderboard_UploadScore));
    engine->RegisterScriptFunction("AGSteam::DownloadScores^1", reinterpret_cast<void*>(SteamLeaderboard_DownloadScores));
    engine->RegisterScriptFunction("AGSteam::geti_LeaderboardNames", reinterpret_cast<void*>(SteamLeaderboard_GetLeaderName));
    engine->RegisterScriptFunction("AGSteam::geti_LeaderboardScores", reinterpret_cast<void*>(SteamLeaderboard_GetLeaderScore));
    engine->RegisterScriptFunction("AGSteam::get_LeaderboardCount", reinterpret_cast<void*>(SteamLeaderboard_GetLeaderCount));
    engine->RegisterScriptFunction("AGSteam::GetCurrentGameLanguage^0", reinterpret_cast<void*>(Steam_GetCurrentGameLanguage));
    engine->RegisterScriptFunction("AGSteam::GetUserName^0", reinterpret_cast<void*>(Steam_GetUserName));
    engine->RequestEventHook(AGSE_FINALSCREENDRAW);
}

void AGS_EngineShutdown()
{
    SteamAPI_Shutdown();
    delete SteamAchievements;
    delete SteamStats;
    delete SteamLeaderboards;
}

int AGS_EngineOnEvent(int event, int data)
{
    if (event == AGSE_FINALSCREENDRAW)
    {
        // use this as main_game_loop
        // the screen is drawn every actual game loop anyway
        SteamAPI_RunCallbacks();
    }
    return 0;
}

// *** END RUN TIME ****
