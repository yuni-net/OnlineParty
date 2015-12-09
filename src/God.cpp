#include "God.h"

namespace OnlineParty
{
	void God::update()
	{
		get_instance().update_dynamic();
	}

	fw::NetSurfer & God::get_surver_surfer()
	{
		return get_instance().surver_surfer;
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

		std::ifstream ifs("data/server_info.txt", std::ios::in);

		const int buffsize = 1024;
		char server_domain[buffsize];
		ifs.get(server_domain, buffsize);

		unsigned short server_port;
		char server_port_text[buffsize];
		ifs.get(server_port_text, buffsize);
		server_port = atoi(server_port_text);

		fw::IP server_ip;
		server_ip.set_by_hostname(server_domain);
		surver_surfer.set(server_ip, server_port);
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