#pragma once

#include <simplect3D.h>
#include "Enemy.h"
#include "Player.h"
#include "Cameraman.h"
#include "Timer.h"

namespace OnlineParty
{
	class God
	{
	public:
		static void update();
		static fw::NetSurfer & get_surver_surfer();
		static float get_elapsed_sec();





	private:
		Timer timer;
		si3::Leaf ground;
		Enemy enemy;
		Player player;
		Cameraman cameraman;
		fw::NetSurfer surver_surfer;

		static God & get_instance();
		God();
		void update_dynamic();
		~God();





		God(const God & god);	// for singleton
		God & operator=(const God & god);	// for singleton
	};
}
