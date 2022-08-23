#include "Node.hpp"
Node::Node(sf::Vector2i pos) {
	this->GlobalDistance = INFINITY;
	this->LocalDistance = INFINITY;
	this->visited = false;
	this->wall = false;
	this->position = pos;
	this->parent = nullptr;
}