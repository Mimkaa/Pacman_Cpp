#include "Pinky.hpp"
Pinky::Pinky(int rad, sf::Color col, float sc, Node* node, Node* corner, Node* original_pos) :Ghost(rad, col, sc, node, corner, original_pos) {
	this->set_seconds_wait(PINKY_NEEDED_SECONDS);
}
void Pinky::get_chase_target(PacMan& pacman, std::vector<Node*>& map_ref) {
	float x = floor(cosf(pacman.angle) + 0.5);
	float y = floor(sinf(pacman.angle) + 0.5);
	sf::Vector2f targer_vector = pacman.position + sf::Vector2f(x * TILESIZE * 4, y * TILESIZE * 4);
	int map_index_x = (int)(targer_vector.x / TILESIZE);
	if (map_index_x > MAP_WIDTH-1) {
		map_index_x = MAP_WIDTH-1;
	}
	else if (map_index_x < 0) {
		map_index_x = 0;
	}
	int map_index_y = (int)(targer_vector.y / TILESIZE);
	if (map_index_y > MAP_HEIGHT-1) {
		map_index_y = MAP_HEIGHT-1;
	}
	else if (map_index_y < 0) {
		map_index_y = 0;
	}

	int map_index = map_index_y * MAP_WIDTH + map_index_x;

	if (!map_ref[map_index]->wall) {
		this->my_chase_target = map_ref[map_index];
	}
	else {
		this->my_chase_target = map_ref[(int)(pacman.position.y / TILESIZE) * MAP_WIDTH + (int)(pacman.position.x / TILESIZE)];
	}
	// if pacman are just facing a wall and not moving
	if (abs(pacman.position.x - pacman.prev_position.x) < 0.001 && abs(pacman.position.y - pacman.prev_position.y) < 0.001) {
		this->my_chase_target = map_ref[(int)(pacman.position.y / TILESIZE) * MAP_WIDTH + (int)(pacman.position.x / TILESIZE)];
	}
}