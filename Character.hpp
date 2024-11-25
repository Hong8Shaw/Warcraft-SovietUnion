#pragma once
#include <easy2d/easy2d.h>
#include <vector>
#include<iostream>

#define Pi 3.141592653589793f
#define WIDTH Window::getWidth()
#define HEIGHT Window::getHeight()
#define Max_Hp 2000.f

using namespace easy2d;

enum class Camp {
	Ally,
	SU
};

class Character: public Sprite {

protected:

	Music* Launching_warning;
	virtual void initBodyShape() = 0;

    /*convert the angular system to radian system concerning the angle.*/
	float getDirection() {
		return _rotation * Pi / -180;
	}

	/*this function aims at amending the position of each vertex on the polygon,
	Primarily differeniate the image into 50 pixel points from the orientation of rows and columns,
	according to the points in the matrix, multiply their corresponding coordinates to obtain the genuine position relative to the image.
	this function assists the function initBodyShape in every subclass.*/
	Shape* createBodyShape(Point* vertices, int n, int dif = 50) {
		for (int i = 0; i < n; i++) {
			vertices[i].x *= getRealWidth() / dif;                      // adjust the x coordinate position.
			vertices[i].y *= getRealHeight() / dif;                     // adjust the y coordinate position.
		}
		return gcnew Shape(Shape::Polygon, vertices, n);
	}

	/*If an individual image png file possesses a series of images,
	this function assists in selecting the corresponding image matching the character's state.*/
	void selectImage(const String& filePath,int total, int order) {
		_image = nullptr;
		setImage(Image::load(filePath));
		crop(Rect(Point(0, (total - order) * getRealHeight() / total),
			Point(getRealWidth(), (total - order + 1) * getRealHeight() / total)));
	}

public:
	Camp camp;//all the character belongs to corresponding camp,the default is SU camp.

	Character(const String& filePath):Sprite(filePath),
		Launching_warning(nullptr),
		camp(Camp::SU) {
		setName(filePath);
		setAnchor(.5f, .5f);	
		setScale(WIDTH / 1500.f);	
		//showBodyShape();
	}

};

/*this class is designed to stipulate the properties of aircrafts in the game. */
class Craft :public Character {
protected:

	ShapeNode* Hp_frame;
	ShapeNode* Hp_bar;
	Sprite Flame;//the explosive flame in Crafts.

	void detectCollision(Camp camp = Camp::Ally) {
		for (auto node : getParent()->getAllChildren()) {
			if (auto Target = dynamic_cast<Craft*>(node)) {
				if (Target->camp == camp && compareWithBody(Target) != BodyRelation::Disjoint)
				{
					decreaseTargetHP(Target,this);//make the target to decrease the Hp.
				}
			}
		}
	}

	/*When crafts are hit, this function offers explosive flame effect on the Sprite*/
	void explosionEffet() {
		if (Hp != Max_Hp) Flame.setVisible(true);
		Flame.setScale(Random::range(1.0f, 3.f));
		Flame.setPos(Random::range(getRealWidth() / 4, 3 * getRealWidth() / 4), Random::range(getRealHeight() / 4, 3 * getRealHeight() / 4));
		Flame.runAction(gcnew Sequence({ gcnew FadeIn(0.1f),gcnew FadeOut(0.2f) }));
	}

public:
	
	float Hp;

	virtual void decreaseTargetHP(Craft* target, Character* attacker) = 0;
	virtual void subtractHp(Character* attacker) = 0;

	/*this constructor is designed for instant explosive craft. for example Iron Fist and rocket.*/
	Craft(const String& filePath) :Character(filePath),
		Hp(Max_Hp),
		Hp_bar(nullptr),
		Hp_frame(nullptr){}

	/*this constructor is designed for main characters that needs only to display the flame.*/
	Craft(const String& filePath, Sprite flame) :Character(filePath),
		Hp(Max_Hp),
		Hp_bar(nullptr),
		Hp_frame(nullptr),
		Flame(flame) {

		Flame.setAnchor(.5f, .5f);
		Flame.setVisible(false);
		addChild(&Flame);
	}

	/*this constructor is designed for main characters that needs to display the Hp bar and flame.*/
	Craft(const String& filePath, Scene* stage, Point hp_pos, Size hp_size, Color color) :
		Character(filePath), 
		Flame(Sprite("explosion.png")), 
		Hp(Max_Hp),
		Hp_frame(gcnew ShapeNode(gcnew Shape(Shape::Rect, Rect(hp_pos, hp_size)))), Hp_bar(gcnew ShapeNode()) {

		Flame.setAnchor(.5f, .5f);
		Flame.setVisible(false);
		addChild(&Flame);

		Hp_bar->setFillColor(color);
		Hp_frame->setFillColor(color);
		//display the Hp monitor at the top of game window.

		stage->addChild(Hp_frame);
		stage->addChild(Hp_bar);

	}
	~Craft() {
		//std::cout << getName() << "is destoryed " << std::endl;
	}
	
};

class Bullet :public Character {

protected:
	
	Character* launcher;//determine which plane launches current bullet.
	
	float velocity;     //define the moving speed of current bullet.
	float displacement;

  /*target: the target current character is attacking.
	attacker: normally it indicates current character itself.*/
	virtual void decreaseTargetHP(Craft* target, Character* attacker) = 0;

	void detectCollision(Camp camp = Camp::Ally) {
		for (auto node : getParent()->getAllChildren()) {
			if (auto Target = dynamic_cast<Craft*>(node)) {
				if (getName() != "SUrocket-explosion2.png" && Target->camp == camp && compareWithBody(Target) != BodyRelation::Disjoint)
				{
					decreaseTargetHP(Target,this);
				}
			}
		}
	}

	void move() {
		collidedWithBoundary();    //remove the bullet when it moves out of game UI realm.
		detectCollision();        
		setPos(getPosX() + velocity * sin(getDirection()),
			   getPosY() + velocity * cos(getDirection()));
	}

	void collidedWithBoundary() {
		if (_pos.x > WIDTH || _pos.x < 0 || _pos.y < 0 || _pos.y > HEIGHT)
			removeSelfInNextUpdate();
	}

public:

	int seq;

	Bullet(const String& filePath, Character* launcher, int seq) :Character(filePath), launcher(launcher), displacement(0), velocity(3.5f), seq(seq){
		_visible = false;
		setScale(WIDTH / 3000.f);
	}
	~Bullet() {
	//std::cout << getName() << "is destoryed " << std::endl;
	}

};
