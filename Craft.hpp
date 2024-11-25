#pragma once
#include "Bullet.hpp"

class Player :public Craft {
private:

	CraftBullet* bullets[16];

	void initBullets(Scene* stage) {
		for (int i = 0; i < std::size(bullets); i++) {
			bullets[i] = new CraftBullet("player_bullet.png",this, -10.f, Point(0, -getHeight() / 2),i * 5);
			stage->addChild(bullets[i]);
		}
	}

	void initBodyShape() {
		Point vertices[] = { Point(25,0),Point(21,5),Point(20,13) ,Point(0,31) ,Point(2,35),
							 Point(20,33),Point(10,46), Point(14,50),Point(25,43),Point(36,50),
							 Point(40,46),Point(30,33),Point(48,35), Point(50,31),Point(30,13),Point(29,5) };
		setBodyShape(createBodyShape(vertices, 16));
	}

	void decreaseTargetHP(Craft* target, Character* attacker) override {
		target->decreaseTargetHP(this,target); //when player collides with enemies, its Hp will decrease too.
		target->subtractHp(this);
	}

	void subtractHp(Character* attacker) {//receive who attacks current instance.

		if (attacker->getName() == "SUcraft.png")
			Hp -= 5.f;

		if (attacker->getName() == "SUcraftBullet.png")
			Hp -= 2.f;

		if (attacker->getName() == "SUrocket-explosion3.png")
			Hp -= 10.f;

		if (attacker->getName() == "diffusebullet.png")
			Hp -= 2.f;

		if (attacker->getName() == "Laser.png")
			Hp -= 5.f;

		if (Hp != Max_Hp)
			Hp_frame->setFillColor(Color::White);

		if (Hp < Max_Hp / 3)
			Hp_bar->setFillColor(Color::Red);
		else
			Hp_bar->setFillColor(Color::Blue);   //recover the Hp.

		explosionEffet();
		Hp_bar->setShape(gcnew Shape(Shape::Rect, Rect(Point(WIDTH / 20, 24 * HEIGHT / 25), Size(Hp * WIDTH / (6 * Max_Hp), HEIGHT / 120))));
	}

	void move() {
		detectCollision(Camp::SU);
		setPos(Input::getMousePos());
		if (_pos.x < 0)
			setPosX(0);
		if (_pos.y < 0)
			setPosY(0);
		if (_pos.x > WIDTH)
			setPosX(WIDTH);
		if (_pos.y > HEIGHT)
			setPosY(HEIGHT);
	}

	void onUpdate() {
		move();
	}

public:

	Player(Scene* stage) :Craft("player.png", stage, Point(WIDTH / 20, 24 * HEIGHT / 25), Size(WIDTH / 6, HEIGHT / 120), Color::Blue) {
		camp = Camp::Ally;
		initBodyShape();
		initBullets(stage);
	}

};

class SUcraft:public Craft {

private:
	CraftBullet* bullets[4];

	void initBodyShape() {
		Point vertices[] = { Point(25,2), Point(18,4), Point(16,5),Point(24,9),Point(22,21),Point(3,15),Point(1,20),Point(21,34),Point(21,39),Point(22,43),
							 Point(25,48),Point(28,43),Point(29,39),Point(29,34),Point(49,20),Point(47,15),Point(28,21),Point(26,9),Point(34,5),Point(32,4) };
		setBodyShape(createBodyShape(vertices, 20));
	}

	void decreaseTargetHP(Craft* target, Character* attacker) override {
		target->subtractHp(attacker);
	}

	void subtractHp(Character* attacker){

		if (attacker->getName() == "player.png")
			Hp -= 500.f;
		if (attacker->getName() == "player_bullet.png")
			Hp -= 200.f;
		explosionEffet();
	}

	void initBullets(Scene* stage) {
		for (int i = 0; i < std::size(bullets); i++) {
			bullets[i] = gcnew CraftBullet("SUcraftBullet.png", this, 8.f, Point(0, +getHeight() / 2), i * 10);
			stage->addChild(bullets[i]);
		}
	}

	/*this function aims at materializing the effect of SUcrafts wandering within the scope*/
	void move() {
		auto pos = Point(Random::range(getWidth() / 2, WIDTH - getWidth() / 2),
			             Random::range(getHeight() / 2, HEIGHT - getHeight()/2));
		runAction(gcnew MoveTo(3.f, pos));
	}

	void onUpdate() {
		if (Hp <= 0.f)
		{
			removeSelfInNextUpdate();
		}
	}

public:

	SUcraft(Scene* stage) :Craft("SUcraft.png",Sprite("explosion.png")) {

		selectImage("SUcraft.png",6,1);
		initBodyShape();	
		initBullets(stage);
		setPos(Random::range(getWidth() / 2, WIDTH - getWidth() / 2), 10 * getHeight());
		//Timer::add([this]() {move();}, 2.0f );
	}

	~SUcraft() {
		for (int i = 0; i < 4; i++) {
			bullets[i]->removeSelfInNextUpdate();
		}
	}
};
