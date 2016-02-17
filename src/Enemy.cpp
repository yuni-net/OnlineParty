#include "Enemy.h"
#include "God.h"
#include "ThunderBolt.h"

namespace OnlineParty
{
	Enemy::Enemy()
	{
		model.load("data/Lat_Miku/Miku_mini.pmd");
		model.x(0.0f);
		model.y(1.0f);
		model.z(0.0f);

		did_init_schedules = false;
		did_send_request_sync_enemy_attack = false;
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

	void Enemy::sync_enemy_attack(const MsRadian schedules[2])
	{
		did_send_request_sync_enemy_attack = false;

		if (did_init_schedules == false)
		{
			schedules_attack[0].ms_radian = schedules[0];
			schedules_attack[0].done = false;
			schedules_attack[1].ms_radian = schedules[1];
			schedules_attack[1].done = false;
			did_init_schedules = true;
			return;
		}

		if (schedules_attack[1].done == false)
		{
			return;
		}

		const bool done_recent = schedules_attack[1].ms_radian.ms == schedules[0].ms;

		schedules_attack[0].ms_radian = schedules[0];
		schedules_attack[0].done = false;
		if (done_recent){ schedules_attack[0].done = true; }
		schedules_attack[1].ms_radian = schedules[1];
		schedules_attack[1].done = false;
	}









	bool Enemy::should_I_sync() const
	{
		if (did_init_schedules == false)
		{
			return true;
		}
		return schedules_attack[1].done && (did_send_request_sync_enemy_attack==false);
	}

	void Enemy::send_request_sync_enemy_attack()
	{
		fw::Bindata request;
		request.add(std::string("{\"signature\": \"OnlineParty\", \"version\": 0, \"request\": \"sync_enemy_attack\"}"));
		God::get_synchronizer().send_request_to_server(request);
		did_send_request_sync_enemy_attack = true;
	}

	bool Enemy::should_I_begin_attack() const
	{
		if (did_init_schedules == false)
		{
			return false;
		}

		if (schedules_attack[0].done == false)
		{
			return God::get_now_time() >= schedules_attack[0].ms_radian.ms;
		}

		if (schedules_attack[1].done == false)
		{
			return God::get_now_time() >= schedules_attack[1].ms_radian.ms;
		}

		return false;
	}

	void Enemy::begin_attack()
	{
		Schedule * schedule = &(schedules_attack[0]);
		if (schedules_attack[0].done)
		{
			schedule = &(schedules_attack[1]);
		}
		std::unique_ptr<ThunderBolt> thunder_bold(new ThunderBolt(schedule->ms_radian.ms, schedule->ms_radian.radian));
		God::get_skill_manager().add(std::move(thunder_bold));
		schedule->done = true;
	}


}