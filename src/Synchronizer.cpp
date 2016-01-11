#include <fstream>
#include "Synchronizer.h"

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

	/**
	@brief I load the config to server_surfer.
	@detail If I failed to load it,  I set the default parameter.
	*/
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
		const int max_member = static_cast<int>(root["max_member"].get<double>());
		members = new MemberP2P[max_member];

		fw::IP server_ip;
		server_ip.set_by_hostname(server_hostname);
		server_surfer.set(server_ip, server_port);
	}

	/**
	@brief I send the join request.
	*/
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

	/**
	@brief I expected to receive the reply for join.
	*/
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

	/**
	@brief I process the requests for synchronization.
	*/
	void Synchronizer::process_request()
	{
		// todo
	}

	/**
	@brief I pop the received data, parse it and set the result to "value".
	@param Set the "picojson::value" object for receive parsed data.
	@return
		true: success
		false: failure
	*/
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
			// todo
			other["ID"].get<double>();
			other["IP"].get<std::string>();
			other["port"].get<double>();
		}
	}


}