#pragma once

#include <memory>
#include "Skill.h"

namespace OnlineParty
{
	class SkillManager
	{
	public:
		void update();
		void add(std::unique_ptr<Skill> skill);
	};
}