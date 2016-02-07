#pragma once

#include "Skill.h"

namespace OnlineParty
{
	class ThunderBolt :public Skill
	{
	public:
		ThunderBolt(const unsigned long long & ms_begin, const float offset_radian);
	};
}