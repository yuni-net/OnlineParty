#include <simplect3D.h>
#include "UserInterface.h"
#include "Player.h"

namespace OnlineParty
{

	UserInterface::UserInterface(Player & my_player):
		myplayer(my_player)
	{
		// todo
	}

	void UserInterface::update()
	{
		if (si3::Manager::key().all_freeing() && si3::Manager::mouse().all_freeing())
		{
			myplayer.on_standby();
		}

		if (si3::Manager::key().pushing(DIK_F) ||
			si3::Manager::key().pushing(DIK_K) ||
			si3::Manager::key().pushing(DIK_RIGHT))
		{
			myplayer.on_slide_right();
		}

		if (si3::Manager::key().pushing(DIK_D) ||
			si3::Manager::key().pushing(DIK_J) ||
			si3::Manager::key().pushing(DIK_LEFT))
		{
			myplayer.on_slide_left();
		}
		// todo
	}
}