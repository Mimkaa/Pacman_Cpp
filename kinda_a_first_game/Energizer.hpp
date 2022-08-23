#pragma once
#include "SimpleRect.hpp"
#include <SFML/Graphics.hpp>
#include "Food.hpp"
class Energizer:public Food
{
public:
	Energizer(sf::Color col, sf::Vector2f pos, int rad);
	
};

