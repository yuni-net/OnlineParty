#pragma once

#include <simplect3D.h>

namespace OnlineParty
{
	class Enemy
	{
	public:
		Enemy();
		void update();
		void sync_enemy_attack(const unsigned long long & ms_to_begin_attack, const float offset_radian);







	private:
		si3::Model model;
		unsigned long long ms_to_begin_attack;
		float offset_radian_attack;
		bool did_begin_attack;

		bool should_I_sync() const;

		/**
		@brief I send the request to synchronize the timing that enemy attack.
		*/
		void send_request_sync_enemy_attack();

		bool should_I_begin_attack() const;
		void begin_attack();
	};
}