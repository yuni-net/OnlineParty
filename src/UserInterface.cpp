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
		// todo
	}
}