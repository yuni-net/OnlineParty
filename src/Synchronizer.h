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
		void send_player_sync_data(const int ID, const fw::Bindata & data) const;



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

		fw::P2P p2p;
		fw::NetSurfer server_surfer;
		int state;
		int my_ID;
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

		/**
		@brief I send the rookie a damy data.
		@param Set the picojson::value of the packet of the reply that the rookie joined.
		@detail When there are restricted corn NAT, both of them must send a data to each other.
				That's why I send it.
		*/
		void send_rookie_damy_data(picojson::value & value) const;

		void sync_enemy_attack(picojson::value & value);
	};
}