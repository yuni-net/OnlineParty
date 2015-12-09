#pragma once

#include <simplect3D.h>

namespace OnlineParty
{
	class Player
	{
	public:
		Player();
		void update();

		si3::Coor3 get_pos() const;





	private:
		si3::Model model;
		float radius;
		float radian;
		float purpose_radian;

		void update_radius();
		void update_radian();
		void update_pos();
		void look_at_center();
	};
}