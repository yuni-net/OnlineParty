#pragma once

#include <memory>
#include <simplect3D.h>

namespace OnlineParty
{
	class RequestRegister
	{
	public:
		RequestRegister(const int max_member);
		void process(fw::P2P & p2p);
		bool is_there_request(const int index) const;
		const fw::Bindata & get_request(const int index) const;


	private:
		std::vector<std::unique_ptr<fw::Bindata> > requests;
	};
}