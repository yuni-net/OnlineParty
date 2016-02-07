#include "Enemy.h"

namespace OnlineParty
{
	Enemy::Enemy()
	{
		model.load("data/nanase_pose.pmd");
		model.x(0.0f);
		model.y(2.0f);
		model.z(0.0f);
	}

	void Enemy::update()
	{
		if (should_I_sync())
		{
			download_enemy_sync_data();
		}

		if (is_it_time_to_begin_attack())
		{
			begin_attack();
		}

		si3::Manager::register_display_object(model);
	}
}