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
		std::unique_ptr<fw::Bindata> request(new fw::Bindata());
		fw::NetSurfer surfer;
		p2p.pop_received_data(*request, surfer);
		/*
		the protocol for synchronization
		"OnlineParty": string

		*/
		// todo
	}

	bool RequestRegister::is_there_request(const int index) const
	{
		return requests[index] != nullptr;
	}

	const fw::Bindata & RequestRegister::get_request(const int index) const
	{
		return *(requests[index]);
	}


}