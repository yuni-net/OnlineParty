#include "God.h"

namespace OnlineParty
{
	void God::update()
	{
		get_instance().update_dynamic();
	}

	const fw::NetSurfer & God::get_surver_surfer()
	{
		return (*get_instance().synchronizer).get_server_surfer();
	}

	float God::get_elapsed_sec()
	{
		return get_instance().timer.get_elapsed_sec();
	}

	unsigned long long God::get_now_time()
	{
		return get_instance().timer.get_now_time();
	}

	int God::get_max_member()
	{
		return get_instance().max_member;
	}

	const Synchronizer & God::get_synchronizer()
	{
		return (*get_instance().synchronizer);
	}

	Player & God::get_player(const int index)
	{
		return get_instance().players[index];
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

		std::ifstream ifs("data/config/God.json", std::ios::in);
		picojson::value value;
		std::string result = picojson::parse(value, ifs);
		if (result.empty() == false)
		{
			// I failed to load config.
			// I set the default parameter.
			max_member = 20;
		}
		else
		{
			picojson::object & root = value.get<picojson::object>();
			max_member = static_cast<int>(root["max_member"].get<double>());
		}

		synchronizer.reset(new Synchronizer());
		players.resize(max_member);
		UI.reset(new UserInterface(get_my_player()));
	}

	void God::update_dynamic()
	{
		timer.update();

		si3::Manager::register_display_object(ground);
		enemy.update();
		(*UI).update();
		for (size_t index = 0; index < players.size(); ++index)
		{
			players[index].update();
		}
		cameraman.update(get_my_player().get_pos());
	}

	God::~God()
	{
		// todo
	}

	Player & God::get_my_player()
	{
		return players[(*synchronizer).get_my_ID()];
	}

}