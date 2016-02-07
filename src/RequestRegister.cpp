#include "RequestRegister.h"
#include "God.h"
#include "MsRadian.h"

namespace OnlineParty
{
	const unsigned int signature_size = 12;	// 12 means strlen("OnlineParty") + 1 as null string.

	RequestRegister::RequestRegister(const int max_member)
	{
		sync_datas.resize(max_member);
	}

	/*
	the protocol for synchronization
	signature "OnlineParty": string
	version: int32_t
	request: string exp. "sync"
	sent-user ID: int32_t
	*/
	void RequestRegister::process(fw::P2P & p2p)
	{
		// The others of latest data is ignore.
		// todo WARNING: There is a vulnerability that the spoofing is possible by declarating the ID
		// Cuz the legitimacy about the relationship between ID and surfer is NOT verified.
		while (p2p.are_there_any_left_datas())
		{
			std::unique_ptr<SyncData> sync_data(new SyncData());
			const bool did_succeed = p2p.pop_received_data(sync_data->request, sync_data->surfer);
			if (did_succeed == false)
			{
				continue;
			}

			char signature[signature_size];
			sync_data->request.copy(signature, signature_size);

			if (signature == std::string("OnlineParty"))
			{
				process_binary_request(std::move(sync_data));
				continue;
			}
			
			picojson::value value;
			const std::string & result = picojson::parse(value, sync_data->request.buffer());
			if (result.empty())
			{
				process_json_request(value);
				continue;
			}

			// That was not the request for OnlineParty.
		}
	}


	bool RequestRegister::is_there_request(const int index) const
	{
		return sync_datas[index] != nullptr;
	}

	fw::Bindata & RequestRegister::get_request(const int index) const
	{
		return sync_datas[index]->request;
	}

	fw::NetSurfer & RequestRegister::get_surfer(const int index) const
	{
		return sync_datas[index]->surfer;
	}



	void RequestRegister::process_binary_request(std::unique_ptr<SyncData> sync_data)
	{
		fw::Bindata & request = sync_data->request;
		request.proceed(signature_size);

		int32_t version;
		sync_data->request >> version;
		if (version == 0)
		{
			process_binary_request_v0(std::move(sync_data));
		}
		else
		{
			// The version is not supported.
		}
	}

	void RequestRegister::process_binary_request_v0(std::unique_ptr<SyncData> sync_data)
	{
		std::string request_text;
		sync_data->request >> request_text;
		if (request_text == "sync")
		{
			int32_t ID;
			sync_data->request >> ID;
			if (static_cast<unsigned int>(ID) >= sync_datas.size())
			{
				return;
			}

			sync_datas[ID] = std::move(sync_data);
		}
	}

	void RequestRegister::process_json_request(picojson::value & value)
	{
		// The program will be terminated when the data is invalid
		// Cuz the keys have not found.
		// "the keys": exp. "signature", "version" and so on.
		picojson::object & root = value.get<picojson::object>();
		if (root["signature"].get<std::string>() != "OnlineParty")
		{
			// This is not OnlineParty data.
			return;
		}

		if (static_cast<int>(root["version"].get<double>()) == 0)
		{
			process_json_request_v0(root);
		}
		else
		{
			// The version is not supported.
		}
	}

	void RequestRegister::process_json_request_v0(picojson::object & root)
	{
		if (root["reply"].get<std::string>() == "sync_enemy_attack")
		{
			picojson::array & json_schedule = root["schedules_enemy_attack"].get<picojson::array>();
			picojson::object & schedule0 = json_schedule[0].get<picojson::object>();
			picojson::object & schedule1 = json_schedule[1].get<picojson::object>();
			MsRadian schedules[2];
			schedules[0].ms = static_cast<unsigned long long>(schedule0["ms"].get<double>());
			schedules[0].radian = static_cast<float>(schedule0["radian"].get<double>());
			schedules[1].ms = static_cast<unsigned long long>(schedule1["ms"].get<double>());
			schedules[1].radian = static_cast<float>(schedule1["radian"].get<double>());
			God::get_enemy().sync_enemy_attack(schedules);
		}
		else
		{
			// This is unknown reply.
		}
	}

}