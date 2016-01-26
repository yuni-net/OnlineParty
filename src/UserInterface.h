#pragma once

namespace OnlineParty
{
	class Player;
	
	class UserInterface
	{
	public:
		UserInterface(Player & my_player);
		void update();



	private:
		Player & myplayer;
	};
}