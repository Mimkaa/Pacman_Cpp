#include "Polygon.hpp"



Polygon::Polygon(unsigned int Points, unsigned int rad, sf::Vector2<float> pos, float ang) {
	this->nPoints = Points;
	this->radius = rad;
	this->position = pos;
	this->angle = ang;
	this->createPoints();
	//this->entity = &ent;// setting the pointer to a new memory address
}


Polygon::~Polygon() {

}

const sf::Vector2f Polygon::get_position() const {
	return this->position;
}
const std::vector<sf::Vector2<float>> Polygon::get_t_points() const {
	return this->transformed;
}
const float Polygon::get_angle() const {
	return this->angle;
}

void Polygon::set_position(sf::Vector2<float> pos) {
	this->position = pos;
}
void Polygon::add_position(sf::Vector2<float> vec) {
	this->position += vec;
}



void Polygon::createPoints() {
	float fTheta = 3.14159f * 2.0f / (float)this->nPoints;
	for (int i = 0; i < this->nPoints; i++) {
		this->original.push_back({ this->radius * cos(fTheta * i), this->radius * sin(fTheta * i) });
		this->transformed.push_back({ this->radius * cos(fTheta * i), this->radius * sin(fTheta * i) });
	}
}

void Polygon::update_verticies() {
	for (int i = 0; i < this->nPoints; i++) {
		this->transformed[i] = {
			(this->original[i].x * cosf(this->angle)) - (this->original[i].y * sinf(this->angle)) + this->position.x,
			(this->original[i].x * sinf(this->angle)) + (this->original[i].y * cosf(this->angle)) + this->position.y,
		};
	}
}

void Polygon::update(sf::Vector2<float> pos) {
	this->position = pos;
	this->update_verticies();

}


void Polygon::rotate(float dAngle) {
	this->angle += dAngle;
}


void Polygon::draw(sf::RenderWindow& i_window) {
	for (int i = 0; i < this->transformed.size() ; i++) {
		sf::Vertex line[] =
		{
			sf::Vertex(this->transformed[i]),
			sf::Vertex(this->transformed[(i+1)% transformed.size()])
			
		};
		line->color = sf::Color(255, 0, 0, 255);
		i_window.draw(line, 2, sf::Lines);
	}
	sf::Vertex direction[] =
	{
		sf::Vertex(this->transformed[0]),
		sf::Vertex(this->position)

	};
	direction->color = sf::Color(255, 0, 0, 255);
	i_window.draw(direction, 2, sf::Lines);
	
}