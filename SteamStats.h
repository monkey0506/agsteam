//
// AGSteam: Steam API Plugin for AGS
// (C) 2011-2017 MonkeyMoto Productions, Inc.
//
#ifndef AGSteam_SteamStats_H
#define AGSteam_SteamStats_H

#include "ags2client/IClientStats.h"

namespace AGSteam
{
	namespace Plugin
	{

		class SteamStats : public AGS2Client::IClientStats
		{
		protected:
			SteamStats() noexcept = default;

		public:
			static SteamStats& GetSteamStats() noexcept;
			~SteamStats() noexcept = default;
			int GetIntStat(char const *name) const noexcept override; // retrieves a stat
			float GetFloatStat(char const *name) const noexcept override;
			float GetAverageRateStat(char const *name) const noexcept override;
			bool SetIntStat(char const *name, int value) const noexcept override; // sets a stat
			bool SetFloatStat(char const *name, float value) const noexcept override;
			bool UpdateAverageRateStat(char const *name, float numerator, float denominator) const noexcept override; // updates average rate with new data
		};

	} // namespace Plugin
} // namespace AGSteam

#endif // AGSteam_SteamStats_H
