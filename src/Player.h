#pragma once

#include <simplect3D.h>

namespace OnlineParty
{
	class Player
	{
	public:
		Player(const si3::ModelData & model_data);
		void init(const int ID, const fw::IP & IP, const unsigned short port);
		void update();

		si3::Coor3 get_pos() const;

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
		float purpose_radian;
		bool should_update;
		

		void update_radius();
		void update_radian();
		void update_pos();
		void look_at_center();
		void update(const float delta_sec);
		void standby(const float delta_sec);
		void attack(const float delta_sec);
		void slide_right(const float delta_sec);
		void slide_left(const float delta_sec);
		bool is_disable() const;
		bool is_afk() const;
		void break_away_from_game();
	};
}