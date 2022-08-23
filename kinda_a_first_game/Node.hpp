#pragma once
#include <SFML/Graphics.hpp>
#include "Settings.hpp"
class Node
{
public:
	sf::Vector2i position;
	std::vector<Node*> Neighbours;
	Node* parent;
	float LocalDistance;
	float GlobalDistance;
	bool visited;
	bool wall;

	Node(sf::Vector2i pos);

};

