#pragma once
#include "Craft.hpp"

class GameOver: public Scene
{
public:
	GameOver() {
	
	}
};

/*All the Sprite moves in the gamePlay scene,which are incorperated in member of gameplay scene std::vector<Node*> _children.*/
class GamePlay :public Scene {

private:

	Player* player;

	void enemiesEmerge(unsigned int capacity) {
		for (int i = 0; i < capacity;i++) {
			auto sucraft = gcnew SUcraft(this);
			addChild(sucraft);
		}
	}

public:
	/*the constructor will contain all the Sprites including background, warcraft, player etc.*/
	GamePlay(){
		enemiesEmerge(3);
		player = gcnew Player(this);
		addChild(player);

	}

};
