//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
#ifndef AGSteam_SteamAchievementss_H
#define AGSteam_SteamAchievementss_H

#include "ags2client/IClientAchievements.h"

namespace AGSteam
{
	namespace Plugin
	{

		class SteamAchievements : public AGS2Client::IClientAchievements
		{
		protected:
			SteamAchievements() noexcept = default;

		public:
			static SteamAchievements& GetSteamAchievements() noexcept;
			~SteamAchievements() noexcept = default;
			bool ResetAchievement(char const *ID) const noexcept override; // clears/resets an achievement
			bool IsAchievementAchieved(char const *ID) const noexcept override; // checks an achievement
			bool SetAchievementAchieved(char const *ID) const noexcept override; // sets an achievement
		};

	} // namespace Plugin
} // namespace AGSteam

#endif // AGSteam_SteamAchievementss_H
