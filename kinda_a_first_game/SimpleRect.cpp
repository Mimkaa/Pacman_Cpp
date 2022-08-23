#include "SimpleRect.hpp"


const float SimpleRect::get_x() const {
	return this->x;
}
const float SimpleRect::get_y() const {
	return this->y;
}
const int SimpleRect::get_width() const {
	return this->width;
}
const int SimpleRect::get_height() const {
	return this->height;
}

SimpleRect::SimpleRect(float v_x, float v_y, int v_width, int v_height) {
	this->x = v_x;
	this->y = v_y;
	this->width = v_width;
	this->height = v_height;
}

SimpleRect::~SimpleRect() {

}
void SimpleRect::set_position(sf::Vector2f pos) {
	this->x = pos.x;
	this->y = pos.y;
}

bool SimpleRect::contains(sf::Vector2f vec) {
	return (vec.x > this->x && vec.y > this->y && vec.x < this->x + this->width  && vec.y < this->y + this->height );
}

bool SimpleRect::collide_rect(SimpleRect& rect) {
	return(this->x < rect.x + rect.width && this->x + this->width > rect.x && this->y < rect.y + rect.height && this->y + this->height > rect.y);
}