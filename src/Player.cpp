#include "Player.h"
#include "God.h"

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
		state = MyState::standingby;
		should_update = true;
	}

	void Player::init(const int ID, const fw::IP & IP, const unsigned short port)
	{
		this->ID = ID;
		surfer.set(IP, port);
		last_sync = 0;
	}

	void Player::update()
	{
		if (is_disable())
		{
			return;
		}

		if (should_update == false)
		{
			should_update = true;
			return;
		}

		if (is_afk())
		{
			break_away_from_game();
			return;
		}
		update(God::get_elapsed_sec());
	}

	si3::Coor3 Player::get_pos() const
	{
		return si3::Coor3(model.x(), model.y(), model.z());
	}


	void Player::on_standby()
	{
		state = MyState::standingby;
	}
	void Player::on_attack()
	{
		state = MyState::attacking;
	}
	void Player::on_slide_right()
	{
		state = MyState::sliding_right;
	}
	void Player::on_slide_left()
	{
		state = MyState::sliding_left;
	}

	/**
	 sync_dataフォーマット
	 float: delta_sec
	 int32: state値
	 float: radius
	 float: radian
	 float: y
	 */
	void Player::evaluate(fw::Bindata & sync_data)
	{
		float delta_sec;
		sync_data >> delta_sec;
		int32_t state;
		sync_data >> state;
		this->state = static_cast<MyState::State>(state);
		float y;
		sync_data >> radius >> radian >> y;
		model.y(y);
		update(delta_sec);
		should_update = false;
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

	void Player::update(const float delta_sec)
	{
		if (state == MyState::standingby)
		{
			standby(delta_sec);
		}
		else if (state == MyState::attacking)
		{
			attack(delta_sec);
		}
		else if (state == MyState::sliding_right)
		{
			slide_right(delta_sec);
		}
		else if (state == MyState::sliding_left)
		{
			slide_left(delta_sec);
		}

		look_at_center();
		si3::Manager::register_display_object(model);
	}


	void Player::standby(const float delta_sec)
	{
		if (radius < default_radius)
		{
			// todo come back
		}
		//todo
	}

	void Player::attack(const float delta_sec)
	{
		//todo
	}

	void Player::slide_right(const float delta_sec)
	{
		//todo
	}

	void Player::slide_left(const float delta_sec)
	{
		//todo
	}

	bool Player::is_disable() const
	{
		return ID == -1;
	}

	bool Player::is_afk() const
	{
		auto gap = God::get_now_time() - last_sync;
		return gap > God::get_synchronizer().get_limit_ms_afk();
	}

	void Player::break_away_from_game()
	{
		ID = -1;
	}

}