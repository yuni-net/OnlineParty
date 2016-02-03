#include "God.h"
#include "../NeckMeasure.h"

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
		player_modeld.load("data/Lat_Miku/Miku_mini.pmd");
		players.resize(max_member);
		for (int index = 0; index < max_member; ++index)
		{
			players[index].reset(new Player(player_modeld));
		}

		fps_measure.reset(new FpsMeasure(get_now_time()));
	}

	void God::update_dynamic()
	{
		timer.update();

		si3::Manager::register_display_object(ground);
		double beg = fw::gettimeofday();
		synchronizer->update();
		double end = fw::gettimeofday();
		double gap = end - beg;
		NeckMeasure::set(0, fw::cnct() << "Synchronizer::updateにかかった時間: " << gap << "秒", gap);

		enemy.update();
		if (UI){ (*UI).update(); }
		beg = fw::gettimeofday();
		for (size_t index = 0; index < players.size(); ++index)
		{
			players[index]->update();
		}
		end = fw::gettimeofday();
		gap = end - beg;
		NeckMeasure::set(1, fw::cnct() << "for(){players[index]->update()}にかかった時間: " << gap << "秒", gap);
		if (UI)
		{
			cameraman.update(get_my_player().get_pos());
		}

		fps_measure->update();
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