#pragma once
#include "Ghost.hpp"
#include <SFML/Graphics.hpp>
#include "Settings.hpp";
class Clyde :public Ghost
{
public:
	int FoodToChase;
	Clyde(int, sf::Color, float, Node*, Node*, Node *);
	void get_chase_target(PacMan& pacman, std::vector<Node*>& map_ref) override;
};

