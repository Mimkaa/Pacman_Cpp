#include "Food.hpp"
Food::Food(sf::Color col, sf::Vector2f pos, int rad) {
	this->color = col;
	this->position = pos;
	this->radius = rad;
	this->initCollRect();
}
Food::~Food() {
	delete this->coll_rect;
}
void Food::initCollRect() {
	this->coll_rect = new SimpleRect(this->position.x - this->radius, this->position.y - this->radius, this->radius * 2 , this->radius * 2);
}
void Food::scale_radius(int scale) {
	this->radius *= scale;
}

void Food::draw(sf::RenderWindow& i_window) {
	sf::CircleShape circle;
	circle.setRadius(this->radius);
	circle.setFillColor(this->color);
	circle.setPosition(this->position.x-this->radius, this->position.y - this->radius);
	
	i_window.draw(circle);

	
}
