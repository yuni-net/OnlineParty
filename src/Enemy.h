#pragma once

#include <simplect3D.h>
#include "MsRadian.h"

namespace OnlineParty
{
	class Enemy
	{
	public:
		Enemy();
		void update();
		void sync_enemy_attack(const MsRadian schedules[2]);







	private:
		struct Schedule
		{
			MsRadian ms_radian;
			bool done;
		};

		si3::Model model;
		Schedule schedules_attack[2];

		bool did_init_schedules;
		bool did_send_request_sync_enemy_attack;

		bool should_I_sync() const;

		/**
		@brief I send the request to synchronize the timing that enemy attack.
		*/
		void send_request_sync_enemy_attack();

		bool should_I_begin_attack() const;
		void begin_attack();
	};
}