#include "Enemy.h"

namespace OnlineParty
{
	Enemy::Enemy()
	{
		model.load("data/Lat_Miku/Miku_mini.pmd");
		model.x(0.0f);
		model.y(1.0f);
		model.z(0.0f);
	}

	void Enemy::update()
	{
		si3::Manager::register_display_object(model);
	}
}