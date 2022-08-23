#include "Inky.hpp"
#include <iostream>
Inky::Inky(int rad, sf::Color col, float sc, Node* node, Node* corner, Node * original_pos) :Ghost(rad, col, sc, node, corner, original_pos) {
	this->set_seconds_wait(INKY_NEEDED_SECONDS);
}

void Inky::get_not_wall(Node*& my_chase_target, Node* current_wall, std::list<Node*>& checked_walls) {
	for (auto neighbour : current_wall->Neighbours) {
		bool found_before = false;
		// check if the node is not in checked walls
		for (auto const& node : checked_walls) {
			if (node == neighbour) {
				found_before = true;
				break;
			}
		}
		if (!found_before) {
			checked_walls.push_back(neighbour);
		}
		if (neighbour->wall == 0) {
			my_chase_target = neighbour;
			break;

		}
		else if (!found_before) {
			this->get_not_wall(my_chase_target, neighbour, checked_walls);
		}


	}
}

void Inky::get_chase_target(PacMan& pacman, std::vector<Node*>& map_ref, const Blinky & blinky) {
	float x = floor(cosf(pacman.angle) + 0.5);
	float y = floor(sinf(pacman.angle) + 0.5);
	sf::Vector2f targer_vector_pacman = pacman.position + sf::Vector2f(x * TILESIZE * 2, y * TILESIZE * 2);
	sf::Vector2f pacman_blinky_vector = targer_vector_pacman - blinky.position;
	pacman_blinky_vector.x *= 2;
	pacman_blinky_vector.y *= 2;
	sf::Vector2f targer_vector = blinky.position + pacman_blinky_vector;
	int map_index_x = (int)(targer_vector.x / TILESIZE);
	if (map_index_x > MAP_WIDTH - 1) {
		map_index_x = MAP_WIDTH - 1;
	}
	else if (map_index_x < 0) {
		map_index_x = 0;
	}
	int map_index_y = (int)(targer_vector.y / TILESIZE);
	if (map_index_y > MAP_HEIGHT - 1) {
		map_index_y = MAP_HEIGHT - 1;
	}
	else if (map_index_y < 0) {
		map_index_y = 0;
	}

	int map_index = map_index_y * MAP_WIDTH + map_index_x;
	if (!map_ref[map_index]->wall) {
		this->my_chase_target = map_ref[map_index];
	}
	else {
		this->my_chase_target = map_ref[map_index];
		std::list<Node*> ToCheckWallsList;
		this->get_not_wall(this->my_chase_target, this->my_chase_target, ToCheckWallsList);
	}
	
	
}