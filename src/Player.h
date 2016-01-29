#pragma once

#include <simplect3D.h>

namespace OnlineParty
{
	class Player
	{
	public:
		Player(const si3::ModelData & model_data);
		void init(const int ID, const fw::IP & IP, const unsigned short port);
		void init(const int ID, const fw::NetSurfer & surfer);
		void update();

		si3::Coor3 get_pos() const;
		const fw::NetSurfer & get_surfer() const;

		/**
		@brief 新しいユーザーが参加したことを検知したときこのAPIを呼び出してください。
		@detail 内部で同期データを送信するフラグが立ちます。
		*/
		void on_lookie_join();

		/**
		 @brief ユーザーからの操作を受け付けるためのAPI
		 */
		void on_standby();
		/**
		@brief ユーザーからの操作を受け付けるためのAPI
		*/
		void on_attack();
		/**
		@brief ユーザーからの操作を受け付けるためのAPI
		*/
		void on_slide_right();
		/**
		@brief ユーザーからの操作を受け付けるためのAPI
		*/
		void on_slide_left();

		/**
		 @brief ほかの端末からの情報に基づいて、現在の状態を算出し、適用する。
		 @warning 必ずパケットのヘッダ部分(プレイヤー番号まで)をpopした状態でデータを渡すこと。
					また、プレイヤー番号のチェックは呼び出し前に必ず行うこと。
		 */
		void evaluate(fw::Bindata & sync_data);

		bool is_disable() const;



	private:
		struct MyState
		{
			enum State
			{
				standingby,
				attacking,
				sliding_right,
				sliding_left
			};
		};

		int ID;
		fw::NetSurfer surfer;
		unsigned long long last_sync;
		MyState::State state;
		si3::Model model;
		float radius;
		float radian;
	//	float purpose_radian;
		bool should_update;
		bool has_changed;
		

		void update_radius();
		void update_radian();
		void update_pos();
		void look_at_center();
		void update(const float delta_sec);
		void standby(const float delta_sec);
		void attack(const float delta_sec);
		void slide_right(const float delta_sec);
		void slide_left(const float delta_sec);
		bool is_afk() const;
		void break_away_from_game();
		bool had_sync_time_elapsed() const;
		void send_sync_data() const;
	};
}