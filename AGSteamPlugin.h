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
// ADDENDUM: BEGINNING ON 2 APRIL 2014 THE FOLLOWING RESTRICTIONS SHALL BE ENFORCED UPON ALL AUTHORIZED THIRD-
// PARTY USAGE, INCLUDING, BUT NOT LIMITED TO SUBLICENSING OR REDISTRIBUTION OF THIS FILE. THESE RESTRICTIONS
// SUPERCEDE AND NULLIFY ALL PRIOR THIRD-PARTY AUTHORIZATIONS GRANTED BY MONKEYMOTO PRODUCTIONS, INC.:
//
//     THE ABOVE NOTICE AND ALL EXISTING ADDENDUMS MUST REMAIN UNCHANGED AT THE BEGINNING OF THIS C++ FILE AND
//     AT THE BEGINNING OF THE LICENSE FILE.
//
//     LIMITED AUTHORIZATION IS GRANTED TO AUTHORIZED THIRD-PARTIES TO REDISTRIBUTE OR SUBLICENSE THIS FILE,
//     PURSUANT TO ALL TERMS INCLUDED HEREIN. ADDITIONAL LICENSES MUST NOT PERMIT LESS RESTRICTIVE USAGE OF
//     THIS FILE THAN ORIGINALLY AUTHORIZED.
//
//     NO FURTHER LIABILITY MUST BE PLACED UPON MONKEYMOTO PRODUCTIONS, INC. BY THE AUTHORIZED THIRD-PARTY,
//     INCLUDING, BUT NOT LIMITED TO, ANY LIABILITY ARISING FROM THIRD-PARTIES NOT DIRECTLY AUTHORIZED BY
//     MONKEYMOTO PRODUCTIONS, INC.
//
//     THE AUTHORIZED THIRD-PARTY ACCEPTS FULL RESPONSIBILITY AND LIABILITY FOR ANY USAGE OF THIS FILE BY
//     THE AUTHORIZED THIRD-PARTY OR OTHER THIRD-PARTIES IT AUTHORIZES, PURSUANT TO THEIR OWN TERMS AND
//     RESTRICTIONS.
//
//     THE AUTHORIZED THIRD-PARTY IS SOLELY RESPONSIBLE FOR VALIDATING THE RIGHTS OF ANY OTHER THIRD-PARTIES
//     THEY WISH TO GRANT ACCESS TO THIS FILE. THIS INCLUDES, BUT IS NOT LIMITED TO, ENSURING THAT THE OTHER
//     PARTY HAS A CURRENT AND VALID NON-DISCLOSURE AGREEMENT WITH VALVE CORPORATION IN ORDER TO GAIN ACCESS
//     TO THE STEAMWORKS SDK AND API.
//
//     IF AT ANY TIME OR FOR ANY REASON AN AUTHORIZED THIRD-PARTY BECOMES INELIGIBLE TO ACCESS THE
//     STEAMWORKS SDK OR API, SUCH AS, BUT NOT LIMITED TO DISSOLUTION OF A NON-DISCLOSURE AGREEMENT WITH
//     VALVE CORPORATION, THE PARTY MUST NOTIFY MONKEYMOTO PRODUCTIONS, INC. WITHIN TWENTY-FOUR HOURS AND
//     IMMEDIATELY DISCONTINUE ALL USAGE OF THIS FILE. AT SUCH TIME AS THE THIRD-PARTY THUS BECOMES SO
//     INELIGIBLE, ALL RIGHTS AND PRIVILEGES GRANTED TO THE PARTY BY MONKEYMOTO PRODUCTIONS, INC. REGARDING
//     THIS FILE BECOME NULL AND VOID.
//
//     ALL OTHER APPLICABLE RESTRICTIONS APPLY. THIS INCLUDES BUT IS NOT LIMITED TO THE TERMS AND CONDITIONS
//     OF NON-DISCLOSURE AGREEMENTS BETWEEN VALVE CORPORATION AND THE AUTHORIZED THIRD-PARTY, AND BETWEEN
//     VALVE CORPORATION AND MONKEYMOTO PRODUCTIONS, INC., THE RESTRICTIONS DETAILED ABOVE, AND LOCAL AND
//     INTERNATIONAL LAWS. MONKEYMOTO PRODUCTIONS, INC. RESERVES ALL RIGHT TO PURSUIT OF LEGAL RECOURSE IN
//     THE EVENT THAT THESE RESTRICTIONS ARE BROKEN BY ANY THIRD-PARTY.
//
// ADDENDUM: IN AGREEMENT WITH WADJET EYE GAMES, THE FOLLOWING PRIVILEGES ARE GRANTED, BEGINNING ON 2 APRIL
// 2014. AUTHORIZED PERSONNEL OF WADJET EYE GAMES ARE HEREBY AUTHORIZED BY MONKEYMOTO PRODUCTIONS, INC. TO
// ACCESS AND MODIFY THIS FILE, PURSUANT TO THE TERMS AND RESTRICTIONS DETAILED ABOVE.
//
// ADDENDUM: IN AGREEMENT WITH INFAMOUS QUESTS, LLC, THE FOLLOWING PRIVILEGES ARE GRANTED, BEGINNING ON 2
// APRIL 2014. AUTHORIZED PERSONNEL OF INFAMOUS QUESTS, LLC ARE HEREBY AUTHORIZED BY MONKEYMOTO PRODUCTIONS,
// INC. TO ACCESS AND MODIFY THIS FILE, PURSUANT TO THE TERMS AND RESTRICTIONS DETAILED ABOVE.
//
// ADDENDUM: IN AGREEMENT WITH PHOENIX ONLINE STUDIOS LLC, THE FOLLOWING PRIVILEGES ARE GRANTED, BEGINNING ON
// 2 APRIL 2014. AUTHORIZED PERSONNEL OF PHOENIX ONLINE STUDIOS LLC ARE HEREBY AUTHORIZED BY MONKEYMOTO
// PRODUCTIONS, INC. TO ACCESS AND MODIFY THIS FILE, PURSUANT TO THE TERMS AND RESTRICTIONS DETAILED ABOVE.
//
#ifndef AGSTEAM_AGSTEAMPLUGIN_H
#define AGSTEAM_AGSTEAMPLUGIN_H

#include "Stub/agsplugin.h"
#include "Stub/IAGSteam.h"
#include "SteamAchievements.h"
#include "SteamLeaderboards.h"
#include "SteamStats.h"
#include "steam/steam_api.h"
#ifdef GetUserName
#undef GetUserName
#endif // GetUserName

IAGSEngine *GetAGSEngine();

namespace AGSteam
{
namespace Plugin
{

class AGSteamPlugin : public Stub::IAGSteam
{
private:
    SteamAchievement *SteamAchievements;
    SteamLeaderboard *SteamLeaderboards;
    SteamStat *SteamStats;

public:
    Stub::ISteamAchievement* GetSteamAchievement()
    {
        return SteamAchievements;
    }

    Stub::ISteamLeaderboard* GetSteamLeaderboard()
    {
        return SteamLeaderboards;
    }

    Stub::ISteamStat* GetSteamStat()
    {
        return SteamStats;
    }

    bool IsInitialized()
    {
        // helper method for the plugin to ensure that the call to Steam is placed before any other Steam functions
        // this function also serves for the AGS property
        static bool steamInitialized = false;
        if (!steamInitialized)
        {
            steamInitialized = SteamAPI_Init();
            if (steamInitialized)
            {
                SteamAchievements = new SteamAchievement;
                SteamLeaderboards = new SteamLeaderboard;
                SteamStats = new SteamStat;
            }
        }
        return steamInitialized;
    }

    void ResetStatsAndAchievements()
    {
        if (!IsInitialized()) return;
        SteamUserStats()->ResetAllStats(true);
    }

    char const* GetCurrentGameLanguage()
    {
        if (!IsInitialized()) return NULL;
        char const *language = SteamApps()->GetCurrentGameLanguage();
        return (language == NULL ? NULL : GetAGSEngine()->CreateScriptString(language));
    }

    char const* GetUserName()
    {
        if (!IsInitialized()) return NULL;
        return GetAGSEngine()->CreateScriptString(SteamFriends()->GetPersonaName()); // GetPersonaName is GUARANTEED to not be NULL
    }

    void Shutdown()
    {
        delete SteamAchievements;
        SteamAchievements = NULL;
        delete SteamLeaderboards;
        SteamLeaderboards = NULL;
        delete SteamStats;
        SteamStats = NULL;
    }

    void Update()
    {
        SteamAPI_RunCallbacks();
    }

    char const* GetAGSScriptHeader()
    {
        return "#define AGSteam_VERSION 3.1\r\n"
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
    }

    char const* GetAGSPluginName()
    {
        return "AGSteam";
    }

    char const* GetAGSPluginDesc()
    {
        return "AGSteam: Steam API Plugin for AGS (C) 2011-2014 MonkeyMoto Productions, Inc.";
    }

    ~AGSteamPlugin()
    {
        Shutdown();
    }
};

AGSteamPlugin& GetAGSteamPlugin();

} // namespace Plugin
} // namespace AGSteam

#endif // AGSTEAM_AGSTEAMPLUGIN_H
