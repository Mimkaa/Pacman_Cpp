#include "Energizer.hpp"
Energizer::Energizer(sf::Color col, sf::Vector2f pos, int rad) :Food(col, pos, rad) {
	this->scale_radius(3);
}
