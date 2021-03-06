#pragma once

#include <memory>
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
		static const int8_t request_code_sync_time = 0;
		static const int8_t request_code_sync_player = 1;

		struct SyncData
		{
			fw::Bindata request;
			fw::NetSurfer surfer;
		};

		fw::P2P p2p;
		fw::NetSurfer server_surfer;
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

		void process_binary_request(std::vector<std::unique_ptr<SyncData> > & sync_datas, std::unique_ptr<SyncData> sync_data);
		void process_binary_request_v0(std::vector<std::unique_ptr<SyncData> > & sync_datas, std::unique_ptr<SyncData> sync_data);
		void process_json_request(std::unique_ptr<picojson::value> value);
		void process_json_request_v0(std::unique_ptr<picojson::value> value);

		void process_sync_datas(std::vector<std::unique_ptr<SyncData> > & sync_datas);
	};
}