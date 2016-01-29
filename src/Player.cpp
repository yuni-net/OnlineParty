#include "Player.h"
#include "God.h"

namespace OnlineParty
{
	static const float slide_speed = 2.0f;
	static const float move_speed = 0.2f;
	static const float default_radius = 10.0f;
	static const float min_radius = 3.0f;
	static const unsigned long long limit_ms_to_sync = 5000;

	Player::Player(const si3::ModelData & model_data)
	{
		model.model_data(model_data);
		ID = -1;
	}

	void Player::init(const int ID, const fw::IP & IP, const unsigned short port)
	{
		fw::NetSurfer surfer;
		surfer.set(IP, port);
		init(ID, surfer);
	}
	void Player::init(const int ID, const fw::NetSurfer & surfer)
	{
		this->ID = ID;
		this->surfer = surfer;
		radian = (fw::xrandom() % 314159265)*0.00000001f;
		radius = default_radius;
		model.y(0.0f);
		update_pos();
		state = MyState::standingby;
		should_update = true;
		has_changed = true;
		last_sync = God::get_now_time();
	}

	void Player::update()
	{
		if (is_disable())
		{
			return;
		}
		if (God::is_my_player(ID)==false)
		{
			if (should_update == false)
			{
				// This player has been already updated by 'evaluate.'
				should_update = true;
				return;
			}
			if (is_afk())
			{
				break_away_from_game();
				return;
			}
		}

		update(God::get_elapsed_sec());

		if (God::is_my_player(ID)==false)
		{
			return;
		}

		if (has_changed || had_sync_time_elapsed())
		{
			send_sync_data();
			has_changed = false;
			last_sync = God::get_now_time();
		}
	}

	si3::Coor3 Player::get_pos() const
	{
		return si3::Coor3(model.x(), model.y(), model.z());
	}

	const fw::NetSurfer & Player::get_surfer() const
	{
		return surfer;
	}

	void Player::on_lookie_join()
	{
		has_changed = true;
	}

	void Player::on_standby()
	{
		if (state == MyState::standingby)
		{
			return;
		}
		state = MyState::standingby;
		has_changed = true;
	}
	void Player::on_attack()
	{
		if (state == MyState::attacking)
		{
			return;
		}
		state = MyState::attacking;
		has_changed = true;
	}
	void Player::on_slide_right()
	{
		if (state == MyState::sliding_right)
		{
			return;
		}
		state = MyState::sliding_right;
		has_changed = true;
	}
	void Player::on_slide_left()
	{
		if (state == MyState::sliding_left)
		{
			return;
		}
		state = MyState::sliding_left;
		has_changed = true;
	}

	/**
	 sync_dataフォーマット
	 uint64: that_time
	 int32: state値
	 float: radius
	 float: radian
	 float: y
	 */
	void Player::evaluate(fw::Bindata & sync_data)
	{
		uint64_t that_time;
		sync_data >> that_time;
		int32_t state;
		sync_data >> state;
		this->state = static_cast<MyState::State>(state);
		float y;
		sync_data >> radius >> radian >> y;
		// debug
		fw::view().set(fw::cnct() << "sent radian for No." << ID << ": " << radian, 10 + ID);
		model.y(y);
		const float delta_sec = static_cast<float>(God::get_now_time() - that_time);
		update(delta_sec);
		should_update = false;
		last_sync = God::get_now_time();
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
		fw::view().set(fw::cnct() << "No."<<ID<<" radian: " << radian, ID);
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

		update_pos();
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
		radian += slide_speed * delta_sec;
		//todo
	}

	void Player::slide_left(const float delta_sec)
	{
		radian -= slide_speed * delta_sec;
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

	bool Player::had_sync_time_elapsed() const
	{
		const auto gap = God::get_now_time() - last_sync;
		return gap >= limit_ms_to_sync;
	}

	/**
	sync_dataフォーマット
	uint64: that_time
	int32: state値
	float: radius
	float: radian
	float: y
	*/
	void Player::send_sync_data() const
	{
		fw::Bindata data;
		data.add(God::get_now_time());
		data.add(state);
		data.add(radius);
		data.add(radian);
		data.add(model.y());
		God::get_synchronizer().send_player_sync_data(ID, data);
	}
}