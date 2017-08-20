# AGSteam: Steam API Plugin for AGS
README FILE

(C) 2011-2017 MonkeyMoto Productions, Inc.

## Getting started

To use the AGSteam plugin, your game must have **already** been accepted for Steam publishing. Otherwise, you won't be able to actually use any of the Steam functions.

The first time you use AGSteam, you will of course need to copy the "AGSteam.dll" to the AGS editor's directory. You will also need to copy the "steam_api.dll" to this folder, as the editor can't load the AGSteam plugin without it. You will only need to repeat this step if upgrading the AGSteam plugin or AGS editor.

In the AGS editor, you can now add the AGSteam plugin to your game.

## Scripting

The way you use the AGSteam plugin will depend on whether you are using a "*unified*" build (the default) or a "*disjoint*" build. You should know this based on which version you downloaded, but an easy way to check is to type "AGS" into a script file and see whether "AGS2Client" or "AGSteam" autocompletes. If "*AGS2Client*" autocompletes, you are using a unified build, or if "*AGSteam*" autocompletes then you are using a disjoint build. In fact, that's really the only difference to how you use the plugin's functions in your scripts. The unified build is designed to make it easier to write code that works with both AGSteam and the AGSGalaxy (GOG Galaxy) plugin. It's up to you which one you decide to use, but if you plan to add your game to GOG Galaxy, then the unified build will facilitate that transition much more easily. From here on, whenever you see "AGS2Client", you can replace it with "AGSteam" if you are using a disjoint build.

### Functions and properties

For a list of functions and properties supported by AGSteam, see [the AGS2Client wiki](https://github.com/monkey0506/ags2client/wiki/Using-the-plugin-interface).

### Helper scripts

While not required, it is recommended that you use helper scripts to wrap any AGSteam functions, which will let you easily disable AGSteam to create a DRM-free build. An example of such a script might look something like this:

*Achievements.ash*

    // Achievements.ash
    
    enum Achievement
    {
      eAchievementFoundKey = 0,
      eAchievementGotPoster,
      eAchievementDopefishLives,
      eAchievement_COUNT
    };
    
    struct Achievements
    {
      import static bool IsAchieved(Achievement);
      import static bool SetAchieved(Achievement);
      import static bool Reset(Achievement);
    };

*Achievements.asc*

    // Achievements.asc
    
    String achievements[eAchievement_COUNT];
    
    function game_start()
    {
      achievements[eAchievementFoundKey] = "FOUND_KEY"; // set the achievement API names, as used by Steam
      achievements[eAchievementGotPoster] = "GOT_POSTER";
      achievements[eAchievementDopefishLives] = "DOPEFISH_LIVES";
    }
    
    static bool Achievements::IsAchieved(Achievement achievement)
    {
      #ifdef AGS2Client_VERSION // only include the following lines if the plugin is in-use
      return AGS2Client.IsAchievementAchieved(achievements[achievement]);
      #endif // AGS2Client_VERSION
      return false; // otherwise, do nothing (plugin not in use)
    }
    
    static bool Achievements::SetAchieved(Achievement achievement)
    {
      #ifdef AGS2Client_VERSION
      return AGS2Client.SetAchievementAchieved(achievements[achievement]);
      #endif // AGS2Client_VERSION
      return false;
    }
    
    static bool Achievements::Reset(Achievement achievement)
    {
      #ifdef AGS2Client_VERSION
      return AGS2Client.ResetAchievement(achievements[achievement]);
      #endif // AGS2Client_VERSION
      return false;
    }


A script such as this also helps by associating an enum with your achievements, so you don't have to remember the achievement's API name if referring to it in more than one place in the script (this could be particularly useful for stats, which are likely to be updated by multiple events).
