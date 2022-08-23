#include "Clyde.hpp"
#include <iostream>
Clyde::Clyde(int rad, sf::Color col, float sc, Node* node, Node* corner, Node* original_pos) :Ghost(rad, col, sc, node, corner, original_pos) {
	this->set_seconds_wait(CLYDE_NEEDED_SECONDS);
}
void Clyde::get_chase_target(PacMan& pacman, std::vector<Node*>& map_ref) {
	if (((this->position.x - pacman.position.x)* (this->position.x - pacman.position.x)+
		(this->position.y - pacman.position.y) * (this->position.y - pacman.position.y)) > (TILESIZE * 8)* (TILESIZE * 8)) {
		this->my_chase_target = map_ref[(int)(pacman.position.y / TILESIZE) * MAP_WIDTH + (int)(pacman.position.x / TILESIZE)];
	}
	else {
		this->my_chase_target = this->my_corner;
	}
}