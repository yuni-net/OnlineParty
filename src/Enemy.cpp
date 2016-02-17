#include "Enemy.h"

namespace OnlineParty
{
	Enemy::Enemy()
	{
		tree.load("data/tree.png");
		tree.scale(0.02f);
		tree.x(10.0f);
		tree.y(tree.height()*0.5f);
		tree.z(10.0f);
		tree.setblend_alpha();
	}

	void Enemy::update()
	{
		si3::Manager::register_display_object(tree);
	}
}