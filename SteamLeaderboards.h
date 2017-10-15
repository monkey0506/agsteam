//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
#ifndef AGSteam_SteamLeaderboards_H
#define AGSteam_SteamLeaderboards_H

#include "ags2client/Cpp11Fix.h"
#include "ags2client/IClientLeaderboards.h"

namespace AGSteam
{
	namespace Plugin
	{
        struct SteamLeaderboards_Statics;

		class SteamLeaderboards : public AGS2Client::IClientLeaderboards
		{
		private:
            friend SteamLeaderboards_Statics;

			DEFAULT_CTOR(SteamLeaderboards);

		public:
			DEFAULT_DTOR(SteamLeaderboards);
			static SteamLeaderboards& GetSteamLeaderboards() noexcept;
			void RequestLeaderboard(char const *leaderboardName, AGS2Client::LeaderboardScore::Type, int limit) const noexcept override;
			bool UploadScore(int score) const noexcept override;
			char const* GetCurrentLeaderboardName() const noexcept override;
			char const* GetLeaderName(int index) const noexcept override;
			int GetLeaderScore(int index) const noexcept override;
			int GetLeaderCount() const noexcept override;
		};
	} // namespace Plugin
} // namespace AGSteam

#endif // AGSteam_SteamLeaderboards_H
