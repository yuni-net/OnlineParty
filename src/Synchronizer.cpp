#include <fstream>
#include "Synchronizer.h"
#include "God.h"
#include "RequestRegister.h"

// todo 満席だったときの処理と、いつまでたってもサーバーから返答が返ってこないときの処理と、そもそもサーバーに接続できないときの処理をする必要がある。

namespace OnlineParty
{

	Synchronizer::Synchronizer()
	{
		state = State::waiting_reply_join;
		load_config();
		send_join_request();
	}

	void Synchronizer::update()
	{
		if (p2p.are_there_any_left_datas() == false)
		{
			return;
		}

		if (state == State::waiting_reply_join)
		{
			wait_reply_join();
		}
		else
		{
			process_request();
		}
	}

	const fw::NetSurfer & Synchronizer::get_server_surfer() const
	{
		return server_surfer;
	}

	int Synchronizer::get_my_ID() const
	{
		return my_ID;
	}

	unsigned long long Synchronizer::get_limit_ms_afk() const
	{
		return limit_ms_afk;
	}

	/*
	the protocol for synchronization
	signature "OnlineParty": string
	version: int32_t
	request: string exp. "sync"
	sent-user ID: int32_t
	*/
	void Synchronizer::send_player_sync_data(const int ID, const fw::Bindata & data) const
	{
		fw::Bindata send_data;
		send_data.add(std::string("OnlineParty"));
		send_data.add(int32_t(0));
		send_data.add(std::string("sync"));
		send_data.add(int32_t(ID));
		send_data.add(data);

		// I send the data 3 times to prevent failure by data-corruption or data-unreached.
		for (int count = 0; count < 3; ++count)
		{
			for (int index = 0; index < God::get_max_member(); ++index)
			{
				if (index == my_ID){ continue; }
				const auto & player = God::get_player(index);
				if (player.is_disable()){ continue; }
				const auto & surfer = player.get_surfer();
				bool did_succeed = p2p.send(surfer, send_data);
			}
		}
	}





	Synchronizer::~Synchronizer()
	{
		// Nothing
	}

	void Synchronizer::load_config()
	{
		std::ifstream ifs("data/config/Synchronizer.json", std::ios::in);
		picojson::value value;
		std::string result = picojson::parse(value, ifs);
		if (result.empty() == false)
		{
			// I failed to load config.
			// I set the default parameter.
			fw::IP server_ip;
			server_ip.set_by_hostname("192.168.11.1");
			server_surfer.set(server_ip, 9696);
			return;
		}

		picojson::object & root = value.get<picojson::object>();
		const std::string & server_hostname = root["server_ip"].get<std::string>();
		const unsigned short server_port = static_cast<unsigned short>(root["server_port"].get<double>());
		fw::IP server_ip;
		server_ip.set_by_hostname(server_hostname);
		server_surfer.set(server_ip, server_port);

		limit_ms_afk = static_cast<unsigned long long>(root["limit_ms_afk"].get<double>());
	}

	void Synchronizer::send_join_request()
	{
		fw::Bindata join_request;
		join_request.add(std::string(
			"{"
				"\"signature\": \"OnlineParty\", "
				"\"version\": 0, "
				"\"request\": \"join\""
			"}"
		));
		p2p.send(server_surfer, join_request);
	}

	void Synchronizer::wait_reply_join()
	{
		while (p2p.are_there_any_left_datas())
		{
			picojson::value value;
			if (get_reply_server(value) == false)
			{
				continue;
			}

			// The program will be terminated when the data is invalid
			// Cuz the keys have not found.
			// "the keys": exp. "signature", "version" and so on.
			picojson::object & root = value.get<picojson::object>();
			if (root["signature"].get<std::string>() != "OnlineParty")
			{
				continue;
			}
			if (static_cast<int>(root["version"].get<double>()) != 0)
			{
				continue;
			}

			if (root["reply"].get<std::string>() == "fully occupied")
			{
				// There are no vacant table.
			}
			else if (root["reply"].get<std::string>() == "join")
			{
				throw_all_left_datas_to_trash();
				save_my_ID(root);
				save_others_surfer(root);
				state = State::waiting_request_sync;
				God::on_join();
			}
		}
	}

	void Synchronizer::process_request()
	{
		// The others of latest data is ignore.
		RequestRegister ter(God::get_max_member());
		ter.process(p2p);
		if (ter.is_there_request_sync_enemy_attack())
		{
			sync_enemy_attack(ter.get_request_sync_enemy_attack());
		}
		if (ter.is_there_reply_rookie_joined())
		{
			send_rookie_damy_data(ter.get_reply_rookie_joined());
		}
		for (int index = 0; index < God::get_max_member(); ++index)
		{
			if (ter.is_there_request(index) == false){ continue; }
			Player & player = God::get_player(index);
			if (player.is_disable())
			{
				// todo
				player.init(index, ter.get_surfer(index));
				God::get_player(get_my_ID()).on_lookie_join();
			}
			player.evaluate(ter.get_request(index));
		}
	}

	bool Synchronizer::get_reply_server(picojson::value & value)
	{
		fw::Bindata data;
		fw::NetSurfer surfer;
		if (p2p.pop_received_data(data, surfer) == false)
		{
			return false;
		}
		if (surfer != server_surfer)
		{
			return false;
		}

		const std::string & result = picojson::parse(value, data.buffer());
		if (result.empty() == false)
		{
			return false;
		}

		return true;
	}

	void Synchronizer::throw_all_left_datas_to_trash()
	{
		fw::Bindata data;
		fw::NetSurfer surfer;
		while (p2p.are_there_any_left_datas())
		{
			p2p.pop_received_data(data, surfer);
		}
	}

	void Synchronizer::save_my_ID(picojson::object & root)
	{
		my_ID = static_cast<int>(root["your ID"].get<double>());
	}

	void Synchronizer::save_others_surfer(picojson::object & root)
	{
		picojson::array & others = root["the others"].get<picojson::array>();
		for (auto rator = others.begin(); rator != others.end(); ++rator)
		{
			picojson::object other = rator->get<picojson::object>();
			const int ID = static_cast<int>(other["ID"].get<double>());
			const std::string IP = other["IP"].get<std::string>();
			const unsigned short port = static_cast<unsigned short>(other["port"].get<double>());
			God::get_player(ID).init(ID, IP, port);
		}
	}

	void Synchronizer::send_rookie_damy_data(picojson::value & value) const
	{
		picojson::object & root = value.get<picojson::object>();
		const int ID = static_cast<int>(root["rookie_ID"].get<double>());
		const std::string & IP = root["rookie_IP"].get<std::string>();
		const unsigned short port = static_cast<unsigned short>(root["rookie_port"].get<double>());
		const fw::NetSurfer surfer(fw::IP(IP), port);
		fw::Bindata data;
		data.add(0);
		p2p.send(surfer, data);
	}

	void Synchronizer::sync_enemy_attack(picojson::value & value)
	{
#if 0
		picojson::object & root = value.get<picojson::object>();
		picojson::array & json_schedule = root["schedules_enemy_attack"].get<picojson::array>();
		picojson::object & schedule0 = json_schedule[0].get<picojson::object>();
		picojson::object & schedule1 = json_schedule[1].get<picojson::object>();
		MsRadian schedules[2];
		schedules[0].ms = static_cast<unsigned long long>(schedule0["ms"].get<double>());
		schedules[0].radian = static_cast<float>(schedule0["radian"].get<double>());
		schedules[1].ms = static_cast<unsigned long long>(schedule1["ms"].get<double>());
		schedules[1].radian = static_cast<float>(schedule1["radian"].get<double>());
		God::get_enemy().sync_enemy_attack(schedules);
#endif
	}

}