#include "God.h"

namespace OnlineParty
{
	void God::update()
	{
		get_instance().update_dynamic();
	}

	const fw::NetSurfer & God::get_surver_surfer()
	{
		return get_instance().synchronizer.get_server_surfer();
	}

	float God::get_elapsed_sec()
	{
		return get_instance().timer.get_elapsed_sec();
	}




	God & God::get_instance()
	{
		static God god;
		return god;
	}

	God::God()
	{
		ground.load("data/ground.png");
		ground.x(0.0f);
		ground.y(0.0f);
		ground.z(0.0f);
		ground.scale(0.1f);
		ground.rot_x(-si3::pi*0.5f);
	}

	void God::update_dynamic()
	{
		timer.update();

		si3::Manager::register_display_object(ground);
		enemy.update();
		player.update();
		cameraman.update(player.get_pos());
	}

	God::~God()
	{
		// todo
	}

}