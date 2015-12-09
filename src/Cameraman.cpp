#include "Cameraman.h"

namespace OnlineParty
{
	static const float distance_camera_player = 6.0f;

	Cameraman::Cameraman()
	{
		si3::Camera & camera = si3::Manager::camera();

		camera.x(0.0f);
		camera.y(2.0f);
		camera.z(6.0f);

		camera.look_x(0.0f);
		camera.look_y(camera.y());
		camera.look_z(0.0f);
	}

	void Cameraman::update(const si3::Coor3 & player_pos)
	{
		si3::Camera & camera = si3::Manager::camera();
		
		si3::Vector3 player_vec(player_pos);
		si3::UniVec3 player_uni_vec(player_vec);
		float distance = player_vec.length() + distance_camera_player;
		si3::Vector3 camera_pos = player_uni_vec*distance;
		camera.x(camera_pos.x());
		camera.z(camera_pos.z());
	}


}