#include <fstream>
#include "Synchronizer.h"
#include "God.h"
#include "RequestRegister.h"

namespace OnlineParty
{

	Synchronizer::Synchronizer()
	{
		members = nullptr;
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

		kick_afk();
	}

	const fw::NetSurfer & Synchronizer::get_server_surfer() const
	{
		return server_surfer;
	}




	Synchronizer::~Synchronizer()
	{
		if (members)
		{
			delete [] members;
		}
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

		max_member = static_cast<int>(root["max_member"].get<double>());
		members = new MemberP2P[max_member];
		init_members();

		limit_ms_afk = static_cast<unsigned long long>(root["limit_ms_afk"].get<double>());
	}

	void Synchronizer::send_join_request()
	{
		fw::Bindata join_request;
		join_request.add(std::string("{"));
		join_request.add(std::string("\"signature\": \"OnlineParty\", "));
		join_request.add(std::string("\"version\": 0, "));
		join_request.add(std::string("\"request\": \"join\", "));
		join_request.add(std::string("}"));
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
			if (root["sigunature"].get<std::string>() != "OnlineParty")
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
			}
		}
	}

	void Synchronizer::process_request()
	{
		// The others of latest data is ignore.
		RequestRegister ter(max_member);
		ter.process(p2p);
		for (int index = 0; index < max_member; ++index)
		{
			if (ter.is_there_request(index) == false){ continue; }
			Player & player = God::get_player(index);
			player.process_request(ter.get_request(index));
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
			auto & member = members[ID];
			auto & surfer = member.surfer;
			fw::IP IP;
			IP.set_by_hostname(other["IP"].get<std::string>());
			const unsigned short port = static_cast<unsigned short>(other["port"].get<double>());

			member.ID = ID;
			surfer.set(IP, port);
			member.last_sync = 0;
		}
	}

	void Synchronizer::init_members()
	{
		for (int index = 0; index < max_member; ++index)
		{
			auto & member = members[index];
			member.ID = -1;
		}
	}

	void Synchronizer::kick_afk()
	{
		for (int index = 0; index < max_member; ++index)
		{
			auto & member = members[index];
			auto gap = God::get_now_time() - member.last_sync;
			if (gap > limit_ms_afk)
			{
				kick_member(index);
			}
		}
	}

	void Synchronizer::kick_member(const int index)
	{
		members[index].ID = -1;
	}

}