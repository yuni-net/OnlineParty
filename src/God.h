#pragma once

#include <simplect3D.h>
#include "Enemy.h"
#include "Player.h"
#include "Cameraman.h"
#include "Timer.h"
#include "Synchronizer.h"
#include "UserInterface.h"
#include "SkillManager.h"

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
		static void on_join();
		static bool is_my_player(const int ID);
		static Enemy & get_enemy();
		static SkillManager & get_skill_manager();





	private:
		Timer timer;
		si3::Leaf ground;
		Enemy enemy;
		si3::ModelData player_modeld;
		si3::ImageData player_pin;
		std::vector<std::unique_ptr<Player> > players;
		Cameraman cameraman;
		std::unique_ptr<Synchronizer> synchronizer;
		int max_member;
		std::unique_ptr<UserInterface> UI;
		SkillManager skill_manager;

		static God & get_instance();
		God();
		void update_dynamic();
		~God();
		Player & get_my_player();
		void on_join_dynamic();





		God(const God & god);	// for singleton
		God & operator=(const God & god);	// for singleton
	};
}
