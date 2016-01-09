#pragma once

#include "picojson.h"
#include <simplect3D.h>

namespace OnlineParty
{
	/**
	This manage all of network communication on this game.
	*/
	class Synchronizer
	{
	public:
		Synchronizer();
		void update();



	private:
		struct State
		{
			enum
			{
				waiting_reply_join,
				waiting_request_sync
			};
		};

		struct MemberP2P
		{
			int ID;
			fw::NetSurfer surfer;
			unsigned long long last_sync;
		};

		fw::P2P p2p;
		fw::NetSurfer server_surfer;
		int state;
		int my_ID;

		void load_config();
		void send_join_request();
		void wait_reply_join();
		void process_request();
		bool get_reply_server(picojson::value & value);
		void throw_all_left_datas_to_trash();
		void save_my_ID(picojson::object & root);
		void save_others_surfer(picojson::object & root);
	};
}