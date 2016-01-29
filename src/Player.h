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
		@brief �V�������[�U�[���Q���������Ƃ����m�����Ƃ�����API���Ăяo���Ă��������B
		@detail �����œ����f�[�^�𑗐M����t���O�������܂��B
		*/
		void on_lookie_join();

		/**
		 @brief ���[�U�[����̑�����󂯕t���邽�߂�API
		 */
		void on_standby();
		/**
		@brief ���[�U�[����̑�����󂯕t���邽�߂�API
		*/
		void on_attack();
		/**
		@brief ���[�U�[����̑�����󂯕t���邽�߂�API
		*/
		void on_slide_right();
		/**
		@brief ���[�U�[����̑�����󂯕t���邽�߂�API
		*/
		void on_slide_left();

		/**
		 @brief �ق��̒[������̏��Ɋ�Â��āA���݂̏�Ԃ��Z�o���A�K�p����B
		 @warning �K���p�P�b�g�̃w�b�_����(�v���C���[�ԍ��܂�)��pop������ԂŃf�[�^��n�����ƁB
					�܂��A�v���C���[�ԍ��̃`�F�b�N�͌Ăяo���O�ɕK���s�����ƁB
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