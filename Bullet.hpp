#pragma once
#include "Character.hpp"


class RocketBullet :public Bullet {

private:

	void initBodyShape() override {
		Point vertices[] = { Point(25,0),
							Point(19,12),Point(17,22) ,Point(9,32) ,Point(3,40),Point(0,46),Point(8,49),
							Point(25,50),
							Point(42,49),Point(50,46),Point(47,40),Point(41,32),Point(33,22),Point(31,12) };
		(seq == 0)?setBodyShape(gcnew Shape(Shape::Circle, Point(getRealWidth()/2,getRealHeight()/2), getRealWidth() / 2)) : setBodyShape(createBodyShape(vertices, 14));
	}

	void decreaseTargetHP(Craft* target, Character* attacker) override {
		removeSelfInNextUpdate();
		target->subtractHp(attacker);
	}

	void onUpdate() {

		displacement += velocity;  //record the displacement of current bullet.

		if (displacement >= 50.f && seq++ == 0)
		{
			diffuse(this, 4, 30.f, seq);
			setRotation(getRotation() + 180.f);
			setImage(Image::load("SUrocket-explosion3.png"));	
		}	
		move();
	}

public:

	//"SUrocket-explosion" + std::to_string(seq + 2) + ".png"
	static void diffuse(Character* launcher, int branch, float difAngle, int seq) {
		for (int i = 0; i < branch; i++) {
			auto bullet = gcnew RocketBullet("SUrocket-explosion" + std::to_string(seq + 2) + ".png", launcher, seq);
			bullet->setRotation((seq == 0) ? i * difAngle / branch : launcher->getRotation() - difAngle / 2 + i * difAngle / (branch - 1));
			bullet->setPos(launcher->getPos());
			bullet->setVisible(true);
			launcher->getParent()->addChild(bullet);
		}
	}

	RocketBullet(const String& filePath,Character* launcher,int seq):Bullet(filePath,launcher,seq) {
		velocity = 2.5f;
		initBodyShape();
	}

};

class CraftBullet :public Bullet {

protected:

	float HitPoint;
	Point pos_diff;

	void initBodyShape() override {
		Point vertices[] = { Point(0,0),Point(50,0), Point(50,50),Point(0,50)};
		setBodyShape(createBodyShape(vertices, 4));
	}	

	void decreaseTargetHP(Craft* target, Character* attacker) override {
		setVisible(false);
		target->subtractHp(attacker);
	}

	void move() {
		displacement += abs(velocity);//record the displacement of current bullet.	
		(launcher->camp == Camp::Ally)? detectCollision(Camp::SU):detectCollision(Camp::Ally);     ///suspend
		reuse();
		setPosY(getPosY() + velocity);
	}

	void onUpdate() {
		switch (seq--) {
		case 0:
			setPos(launcher->getPos() - Point(0, launcher->getHeight() / 2));
			setVisible(true);
			break;
		case -1:
			seq = -1;
			move();
		}
	}

public:

	CraftBullet(const String& filePath, Character* launcher, float velocity,Point pos ,int seq) :
		Bullet(filePath, launcher, seq),
		HitPoint(5.f),
		pos_diff(pos){

		this->velocity = velocity;
		Launching_warning = MusicPlayer::preload("playershooting.wav");
		initBodyShape();
	}

	~CraftBullet() {
		std::cout << "bullet is removed" << std::endl;
	}

	void reuse() {                                        // when the bullet exceeds the specific displacement, it will be reused by the player,which could helps to reduce the freqency of allocating memeory.
		if (displacement >= HEIGHT) {
			setPos(launcher->getPos() + pos_diff);        //Point(0, launcher->getHeight() / 2)
			setVisible(true);
			displacement = 0;
		}
	}

};
