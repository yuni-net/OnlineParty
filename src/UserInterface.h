#pragma once

namespace OnlineParty
{
	class Player;
	
	class UserInterface
	{
	public:
		UserInterface();
		UserInterface(Player & myplayer);
		void update();
	};
}