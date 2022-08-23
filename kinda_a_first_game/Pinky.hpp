#pragma once
#include "Ghost.hpp"
#include <SFML/Graphics.hpp>
class Pinky :public Ghost
{
public:
	int FoodToChase;
	Pinky(int, sf::Color, float, Node*, Node*, Node*);
	void get_chase_target(PacMan& pacman, std::vector<Node*>& map_ref) override;
};
