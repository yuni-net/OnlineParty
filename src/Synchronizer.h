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
		const fw::NetSurfer & get_server_surfer() const;
		int get_my_ID() const;
		unsigned long long get_limit_ms_afk() const;



		~Synchronizer();
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
			int ID;	// -1 means invalid member.
			fw::NetSurfer surfer;
			unsigned long long last_sync;
		};

		fw::P2P p2p;
		fw::NetSurfer server_surfer;
		int state;
		int my_ID;
	//	MemberP2P * members;
	//	int max_member;
		unsigned long long limit_ms_afk;

		/**
		@brief I load the config to server_surfer.
		@detail If I failed to load it,  I set the default parameter.
		*/
		void load_config();

		/**
		@brief I send the join request.
		*/
		void send_join_request();

		/**
		@brief I expected to receive the reply for join.
		*/
		void wait_reply_join();

		/**
		@brief I process the requests for synchronization.
		*/
		void process_request();

		/**
		@brief I pop the received data, parse it and set the result to "value".
		@param Set the "picojson::value" object for receive parsed data.
		@return
		true: success
		false: failure
		*/
		bool get_reply_server(picojson::value & value);

		void throw_all_left_datas_to_trash();
		void save_my_ID(picojson::object & root);
		void save_others_surfer(picojson::object & root);
	//	void init_members();
	//	void kick_afk();
	//	void kick_member(const int index);
	};
}