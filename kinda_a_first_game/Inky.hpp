#pragma once
#include "Ghost.hpp"
#include <SFML/Graphics.hpp>
#include "Blinky.hpp"
#include "Settings.hpp"
#include <list>


class Inky :public Ghost
{
public:
	int FoodToChase;
	Inky(int, sf::Color, float, Node*, Node*, Node*);
	void get_chase_target(PacMan& pacman, std::vector<Node*>& map_ref, const Blinky& blinky) ;
	void get_not_wall(Node*& my_chase_target, Node* current_wall, std::list<Node*>& checked_walls);
};

