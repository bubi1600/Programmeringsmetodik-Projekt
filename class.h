#pragma once
#include<iostream>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
#include<math.h>
#include<cstdlib>
#include<vector>
using namespace sf;

/**
 * Bullet class 
 * size and pos of the bullet.
 */
class Bullet
{
public:
	Sprite shape;

	Bullet(Texture* texture, Vector2f pos)
	{
		this->shape.setTexture(*texture);

		this->shape.setScale(0.07f, 0.07f);

		this->shape.setRotation(90);

		this->shape.setPosition(pos);
	}

	~Bullet() {}
};

/**
 * Player class
 * maxhp and size of player
 * include class bullet as vector for bullets
 * .
 */
class Player
{
public:
	Sprite shape;
	Texture* texture;

	int HP;
	int HPMax;

	std::vector<Bullet> bullets;

	Player(Texture* texture)
	{
		this->HPMax = 10;
		this->HP = this->HPMax;

		this->texture = texture;
		this->shape.setTexture(*texture);

		this->shape.setScale(0.1f, 0.1f);

		this->shape.setRotation(270);

		this->shape.setPosition(370, 600);
	}

	~Player() {}

};

/**
 * Enemy class
 * hp and size of enemy
 * and pos for random enemy spawn in y-cord.
 */
class Enemy
{
public:
	Sprite shape;

	int HP;
	int HPMax;

	Enemy(Texture* texture, Vector2u windowSize)
	{
		this->HPMax = 1;
		this->HP = this->HPMax;

		this->shape.setTexture(*texture);

		this->shape.setScale(0.1f, 0.1f);

		this->shape.setRotation(270);

		this->shape.setPosition(rand() % (int)windowSize.x - this->shape.getGlobalBounds().width, 0);
	}

	~Enemy() {}
};