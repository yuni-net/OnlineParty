#include "Enemy.h"
#include "God.h"

namespace OnlineParty
{
	Enemy::Enemy()
	{
		model.load("data/nanase_pose.pmd");
		model.x(0.0f);
		model.y(2.0f);
		model.z(0.0f);

		ms_to_begin_attack = 0;
		did_begin_attack = true;
	}

	void Enemy::update()
	{
		if (should_I_sync())
		{
			send_request_sync_enemy_attack();
		}

		if (should_I_begin_attack())
		{
			begin_attack();
		}

		si3::Manager::register_display_object(model);
	}

	void Enemy::sync_enemy_attack(const unsigned long long & ms_to_begin_attack, const float offset_radian)
	{
		if (ms_to_begin_attack <= this->ms_to_begin_attack)
		{
			return;
		}

		if (did_begin_attack == false)
		{
			begin_attack();
		}

		this->ms_to_begin_attack = ms_to_begin_attack;
		offset_radian_attack = offset_radian;
	}









	bool Enemy::should_I_sync() const
	{
		return did_begin_attack;
	}

	void Enemy::send_request_sync_enemy_attack()
	{
		fw::Bindata request;
		request.add(std::string("{\"signature\": \"OnlineParty\", \"version\": 0, \"request\": \"sync_enemy_attack\"}"));
		God::get_synchronizer().send_request_to_server(request);
	}

	bool Enemy::should_I_begin_attack() const
	{
		if (did_begin_attack)
		{
			return false;
		}
		return ms_to_begin_attack <= God::get_now_time();
	}

	void Enemy::begin_attack()
	{
		// todo
	}


}