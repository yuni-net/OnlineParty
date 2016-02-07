#pragma once

#include <memory>
#include "picojson.h"
#include <simplect3D.h>

namespace OnlineParty
{
	/**
	I analyze the received datas and register it as needed.
	*/
	class RequestRegister
	{
	public:
		RequestRegister(const int max_member);
		void process(fw::P2P & p2p);
		bool is_there_request(const int index) const;
		fw::Bindata & get_request(const int index) const;
		fw::NetSurfer & get_surfer(const int index) const;


	private:
		struct SyncData
		{
			fw::Bindata request;
			fw::NetSurfer surfer;
		};

		std::vector<std::unique_ptr<SyncData> > sync_datas;


		void process_binary_request(std::unique_ptr<SyncData> sync_data);
		void process_binary_request_v0(std::unique_ptr<SyncData> sync_data);
		void process_json_request(picojson::value & value);
		void process_json_request_v0(picojson::object & root);
	};
}