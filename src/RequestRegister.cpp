#include "RequestRegister.h"

namespace OnlineParty
{
	RequestRegister::RequestRegister(const int max_member)
	{
		requests.resize(max_member);
	}

	void RequestRegister::process(fw::P2P & p2p)
	{
		// The others of latest data is ignore.
		// todo WARNING: There is a vulnerability that the spoofing is possible by declarating the ID
		// Cuz the legitimacy about the relationship between ID and surfer is NOT verified.
		while (p2p.are_there_any_left_datas())
		{
			std::unique_ptr<fw::Bindata> request(new fw::Bindata());
			fw::NetSurfer surfer;
			p2p.pop_received_data(*request, surfer);
			/*
			the protocol for synchronization
			signature "OnlineParty": string
			version: int32_t
			request: string exp. "sync"
			sent-user ID: int32_t
			*/
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
				process_v0(std::move(request));
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





	void RequestRegister::process_v0(std::unique_ptr<fw::Bindata> request)
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
		}
	}
}