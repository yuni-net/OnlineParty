#include "RequestRegister.h"
#include "../NeckMeasure.h"

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
		static int max_fail = 0;
		int fail_count = 0;
		static int max_count = 0;
		int count = 0;
		NeckMeasure::init();
		// The others of latest data is ignore.
		// todo WARNING: There is a vulnerability that the spoofing is possible by declarating the ID
		// Cuz the legitimacy about the relationship between ID and surfer is NOT verified.
		while (true)
		{
			++count;
			double beg = fw::gettimeofday();
			const bool are_there = p2p.are_there_any_left_datas();
			double end = fw::gettimeofday();
			double gap = end - beg;
			NeckMeasure::set(0, fw::cnct() << "p2p.are_there_any_left_datas()‚É‚©‚©‚Á‚½ŽžŠÔ: " << gap << "•b", gap);
			NeckMeasure::each_frame();
			if (are_there == false){ break; }

			std::unique_ptr<fw::Bindata> request(new fw::Bindata());
			std::unique_ptr<fw::NetSurfer> surfer(new fw::NetSurfer());
			// todo bugfix
			// 'p2p.pop_received_data' will be failed in the case:
			//     * the other player exit.
			//     * I join the room which the other player has just exit.
			beg = fw::gettimeofday();
			const bool did_succeed = p2p.pop_received_data(*request, *surfer);
			end = fw::gettimeofday();
			gap = end - beg;
			NeckMeasure::set(1, fw::cnct() << "p2p.pop_received_data(*request, *surfer)‚É‚©‚©‚Á‚½ŽžŠÔ: " << gap << "•b", gap);
			if (did_succeed == false)
			{
				++fail_count;
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
				beg = fw::gettimeofday();
				process_v0(std::move(request), std::move(surfer));
				end = fw::gettimeofday();
				gap = end - beg;
				NeckMeasure::set(2, fw::cnct() << "process_v0(std::move(request), std::move(surfer))‚É‚©‚©‚Á‚½ŽžŠÔ: " << gap << "•b", gap);
			}
		}

		if (count > max_count)
		{
			max_count = count;
		}
		fw::view().set(fw::cnct() << "¡‰ñ‚Ìwhileƒ‹[ƒv”: " << count, 6);
		fw::view().set(fw::cnct() << "Å‘åwhileƒ‹[ƒv”: " << max_count, 7);

		if (fail_count > max_fail)
		{
			max_fail = fail_count;
		}
		fw::view().set(fw::cnct() << "max of p2p.pop_received_data: " << max_fail, 8);
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