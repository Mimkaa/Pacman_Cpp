#pragma once
#include "SimpleRect.hpp"
#include <SFML/Graphics.hpp>
class Food
{
	sf::Color color;
	int radius;
public:
	sf::Vector2f position;
	SimpleRect* coll_rect;
	Food(sf::Color col, sf::Vector2f pos, int rad);
	virtual ~Food();
	void draw(sf::RenderWindow& i_window);
	void initCollRect();
	void scale_radius(int scale);
};

