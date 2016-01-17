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