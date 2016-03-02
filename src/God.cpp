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
		return get_instance().timer->get_elapsed_sec();
	}

	unsigned long long God::get_now_time()
	{
		return get_instance().timer->get_now_time();
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
		return *(get_instance().players[index]);
	}

	void God::on_join()
	{
		get_instance().on_join_dynamic();
	}

	bool God::is_my_player(const int ID)
	{
		return ID == get_instance().synchronizer->get_my_ID();
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
		timer.reset(new Timer());
		timer->begin_sync();
		player_modeld.load("data/Lat_Miku/Miku_mini.pmd");
		player_pin.load("data/player_pin.png", 100.0f);
		players.resize(max_member);
		for (int index = 0; index < max_member; ++index)
		{
			players[index].reset(new Player(player_modeld, player_pin));
		}
	}

	void God::update_dynamic()
	{
		timer->update();

		si3::Manager::register_display_object(ground);
		synchronizer->update();
		enemy.update();
		if (UI){ (*UI).update(); }
		for (size_t index = 0; index < players.size(); ++index)
		{
			players[index]->update();
		}
		if (UI)
		{
			cameraman.update(get_my_player().get_pos());
		}
	}

	God::~God()
	{
		// todo
	}

	Player & God::get_my_player()
	{
		return *(players[(*synchronizer).get_my_ID()]);
	}

	void God::on_join_dynamic()
	{
		get_my_player().init(synchronizer->get_my_ID(), fw::IP(), 0);
		UI.reset(new UserInterface(get_my_player()));
	}
}