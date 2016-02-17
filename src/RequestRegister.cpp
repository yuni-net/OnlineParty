#include "RequestRegister.h"
#include "God.h"

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

			std::unique_ptr<picojson::value> value(new picojson::value());
			const std::string & result = picojson::parse(*value, sync_data->request.buffer());
			if (result.empty())
			{
				process_json_request(std::move(value));
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

	bool RequestRegister::is_there_reply_rookie_joined() const
	{
		return reply_rookie_joined.get() != nullptr;
	}

	picojson::value & RequestRegister::get_reply_rookie_joined()
	{
		return *reply_rookie_joined;
	}

	bool RequestRegister::is_there_request_sync_enemy_attack() const
	{
		return request_sync_enemy_attack.get() != nullptr;
	}

	picojson::value & RequestRegister::get_request_sync_enemy_attack()
	{
		return *request_sync_enemy_attack;
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

	void RequestRegister::process_json_request(std::unique_ptr<picojson::value> value)
	{
		// The program will be terminated when the data is invalid
		// Cuz the keys have not found.
		// "the keys": exp. "signature", "version" and so on.
		picojson::object & root = value->get<picojson::object>();
		if (root["signature"].get<std::string>() != "OnlineParty")
		{
			// This is not OnlineParty data.
			return;
		}

		if (static_cast<int>(root["version"].get<double>()) == 0)
		{
			process_json_request_v0(std::move(value));
		}
		else
		{
			// The version is not supported.
		}
	}

	void RequestRegister::process_json_request_v0(std::unique_ptr<picojson::value> value)
	{
		picojson::object & root = value->get<picojson::object>();
		const std::string & reply = root["reply"].get<std::string>();
		if (reply == "sync_enemy_attack")
		{
			request_sync_enemy_attack = std::move(value);
		}
		else if (reply == "rookie_joined")
		{
			reply_rookie_joined = std::move(value);
		}
		else
		{
			// This is unknown reply.
		}
	}

}