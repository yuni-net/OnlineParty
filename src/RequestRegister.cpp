#include "RequestRegister.h"

namespace OnlineParty
{
	RequestRegister::RequestRegister(const int max_member)
	{
		requests.resize(max_member);
		surfers.resize(max_member);
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
			std::unique_ptr<fw::Bindata> request(new fw::Bindata());
			std::unique_ptr<fw::NetSurfer> surfer(new fw::NetSurfer());
			// todo bugfix
			// 'p2p.pop_received_data' will be failed in the case:
			//     * the other player exit.
			//     * I join the room which the other player has just exit.
			const bool did_succeed = p2p.pop_received_data(*request, *surfer);
			if (did_succeed == false)
			{
				continue;
			}

			const char * signature = (*request).buffer();
			if (signature != std::string("OnlineParty"))
			{
				// This is NOT OnlineParty data.
				continue;
			}

			std::string signature_box;
			(*request) >> signature_box;

			int32_t version;
			(*request) >> version;
			if (version == 0)
			{
				process_v0(std::move(request), std::move(surfer));
			}
		}
	}

	bool RequestRegister::is_there_request(const int index) const
	{
		return requests[index] != nullptr;
	}

	fw::Bindata & RequestRegister::get_request(const int index) const
	{
		return *(requests[index]);
	}

	fw::NetSurfer & RequestRegister::get_surfer(const int index) const
	{
		return *(surfers[index]);
	}



	void RequestRegister::process_v0(std::unique_ptr<fw::Bindata> request, std::unique_ptr<fw::NetSurfer> surfer)
	{
		std::string request_text;
		(*request) >> request_text;
		if (request_text == "sync")
		{
			int32_t ID;
			(*request) >> ID;
			if (static_cast<unsigned int>(ID) >= requests.size())
			{
				return;
			}

			requests[ID] = std::move(request);
			surfers[ID] = std::move(surfer);
		}
	}
}