#include "Player.h"

namespace OnlineParty
{
	static const float move_speed = 0.2f;
	static const float default_radius = 10.0f;
	static const float min_radius = 3.0f;

	Player::Player()
	{
		model.load("data/Lat_Miku/Miku_mini.pmd");
		radian = (fw::xrandom() % 314159265)*0.00000001f;
		purpose_radian = radian;
		radius = default_radius;
		model.y(0.0f);
		update_pos();
	}

	void Player::update()
	{
		auto & mouse = si3::Manager::mouse();
		if (mouse.pushing(si3::Mouse::left) &&
			mouse.pushing(si3::Mouse::right))
		{
			update_radius();
		}
		else if(mouse.pushing(si3::Mouse::left) ||
				mouse.pushing(si3::Mouse::right))
		{
			update_radian();
		}
		else
		{

		}
		update_pos();
		look_at_center();
		si3::Manager::register_display_object(model);
	}

	si3::Coor3 Player::get_pos() const
	{
		return si3::Coor3(model.x(), model.y(), model.z());
	}


	void Player::update_radius()
	{
		if (radius <= min_radius)
		{
			return;
		}

		if (radius - move_speed < min_radius)
		{
			radius = min_radius;
		}
		else
		{
			radius -= move_speed;
		}
	}

	void Player::update_radian()
	{
		auto & mouse = si3::Manager::mouse();
		if (mouse.pushing(si3::Mouse::left))
		{
			radian -= move_speed;
		}
		if (mouse.pushing(si3::Mouse::right))
		{
			radian += move_speed;
		}
	}

	void Player::update_pos()
	{
		model.x(cos(radian)*radius);
		model.z(sin(radian)*radius);
	}

	void Player::look_at_center()
	{
		const float anti_radian = fw::xy2radian(model.x(), model.z());
		const float radian = anti_radian + si3::pi + si3::pi*0.5f;
		model.radian_y(radian);
	}
}