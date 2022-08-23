#include "Blinky.hpp"
#include <iostream>
Blinky::Blinky(int rad, sf::Color col, float sc, Node* node, Node* corner, Node* original_pos):Ghost(rad, col, sc, node, corner, original_pos) {
	this->set_seconds_wait( BLINKY_NEEDED_SECONDS);
}
void Blinky::get_chase_target(PacMan& pacman,std::vector<Node*>& map_ref) {
	this->my_chase_target = map_ref[(int)(pacman.position.y / TILESIZE) * MAP_WIDTH + (int)(pacman.position.x / TILESIZE)];
}
