#pragma once

#include <simplect3D.h>
#include "Enemy.h"
#include "Player.h"
#include "Cameraman.h"
#include "Timer.h"
#include "Synchronizer.h"

namespace OnlineParty
{
	class God
	{
	public:
		static void update();
		static const fw::NetSurfer & get_surver_surfer();
		static float get_elapsed_sec();
		static unsigned long long get_now_time();
		static int get_max_member();
		static const Synchronizer & get_synchronizer();
		static Player & get_player(const int index);





	private:
		Timer timer;
		si3::Leaf ground;
		Enemy enemy;
		std::vector<Player> players;
		Cameraman cameraman;
		std::unique_ptr<Synchronizer> synchronizer;
		int max_member;

		static God & get_instance();
		God();
		void update_dynamic();
		~God();





		God(const God & god);	// for singleton
		God & operator=(const God & god);	// for singleton
	};
}
