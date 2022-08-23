#include "PacMan.hpp"
#include <iostream>
#include "Settings.hpp"






//Constructor and Destructor
PacMan::PacMan(sf::Vector2<float> pos, int vec_UpperJaw,
	int vec_LowerJaw, unsigned int numPoints, unsigned int rad, sf::Color col, float ang) {
	this->position = pos;
	this->LowerJawInd = vec_LowerJaw;
	this->UpperJawInd = vec_UpperJaw;
	this->nPoints = numPoints;
	this->radius = rad;
	this->color = col;
	this->angle = ang;
	this->nom_dir = 1;
	this->last_update = 0;
	this->current_time = 0;
	this->rect = nullptr;
	this->coll_rect = nullptr;
	this->initialize_rects();
	this->my_node = nullptr;
	this->prev_position = sf::Vector2<float>(0.f, 0.f);
	this->score = 0;
	this->object_coll_rect = new SimpleRect(this->position.x - this->radius, this->position.y - this->radius, this->radius * 2, this->radius * 2);
	this->eaten = false;
	this->original_pos = pos;
}

PacMan::~PacMan(){
	
	delete this->rect;
	delete this->coll_rect;
	delete this->object_coll_rect;
	
}

// Getters and Setters
void PacMan::set_position(sf::Vector2<float> vec) {
	this->position.x = vec.x;
	this->position.y = vec.y;
}
const sf::Vector2<float> PacMan::get_position() const {
	return this->position;
}
Polygon* PacMan::get_rect() const {
	return this->rect;
}

void PacMan::add_position(sf::Vector2<float> vec) {
	this->position += vec;
}

//Functional

void PacMan::initialize_rects() {

	this->rect = new Polygon(4, (int)(this->radius / sqrt(2))*2, this->position, PI/4);
	this->coll_rect = new SimpleRect(this->position.x, this->position.y, this->radius * 6, this->radius * 6);
}

bool PacMan::FloatComp(float a, float b, float epsilon) {
	return fabs(a - b) < epsilon;
}


void PacMan::createPoints() {
	float fTheta = 3.14159f * 2.0f / (float)this->nPoints;
	for (int i = 0; i < this->nPoints; i++) {
		this->original.push_back({ this->radius * cos(fTheta * i), this->radius * sin(fTheta * i) });
		this->transformed.push_back({ this->radius * cos(fTheta * i), this->radius * sin(fTheta * i) });
	}
}

void PacMan::move_jaws() {
	this->current_time += 1;
	if (this->current_time - this->last_update > 2) {
		this->last_update = this->current_time;
		this->LowerJawInd += 1 * this->nom_dir;
		this->UpperJawInd -= 1 * this->nom_dir;
		this->UpperJawInd = (this->UpperJawInd + this->transformed.size()) % this->transformed.size();
	}
	int mouth_extend = 4;
	if (this->eaten) {
		mouth_extend = 1;
	}
	if (this->LowerJawInd > transformed.size() / mouth_extend - 1 || this->LowerJawInd == 0) {
		this->nom_dir *= -1;
	}
}

void PacMan::animate() {
	std::vector<sf::Vector2<float>> trs_copy(this->transformed);
	this->drawable = trs_copy;
	
	
	if (this->LowerJawInd > 0) {
		
		int drawable_length = this->drawable.size();
		// appending vectors that we want to pop
		std::vector<int> to_erase;
		for (int i = 1; i < this->LowerJawInd; i++) {
			to_erase.push_back(i);
			to_erase.push_back((this->drawable.size()) - i );
			/*std::cout << i << std::endl;
			std::cout << (this->drawable.size() - 1) - i << std::endl;*/
		}
		//remove vectors from drawable by indecies we recorded in to_erase
		std::vector<sf::Vector2<float>> new_drawable;
		if (!to_erase.empty()) {

			for (int i = 0; i < this->drawable.size(); i++) {
				bool in_erase = false;
				for (int j = 0; j < to_erase.size(); j++) {
					if (i == to_erase[j]) {
						in_erase = true;
					}
					
				}
				if (!in_erase) {
					new_drawable.push_back(this->drawable[i]);
				}
			}

			this->drawable = new_drawable;
			
		}
		// adjust jaw notch
		if (this->LowerJawInd > 0) {
			this->drawable[0] = this->position;
		}
		else {
			this->drawable[0] = this->transformed[0];
		}
	}
	
	
	
}

void PacMan::get_keys() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		this->angle = 3.14159;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		this->angle = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		this->angle = 4.71239;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		this->angle = 1.5708;
	}
}

void PacMan::move(float dt) {
	if (!this->drawable.empty() && ! this->transformed.empty() && ! this->eaten) {
		float x = floor(cosf(this->angle) + 0.5);
		float y = floor(sinf(this->angle) + 0.5);
		x *= PACMANSPEED * dt;
		y *= PACMANSPEED * dt;
		this->position += sf::Vector2<float>(x, y);
	}
	


}

void PacMan::wrap_around() {
	if (this->position.x > SCREEN_WIDTH) {
		this->position.x = 0;
	}
	else if (this->position.x < 0) {
		this->position.x = SCREEN_WIDTH;
	}
	if (this->position.y > SCREEN_HEIGHT) {
		this->position.y = 0;
	}
	else if (this->position.y < 0) {
		this->position.y = SCREEN_HEIGHT;
	}

}

void PacMan::update_verticies() {
	// tansform in world space
	for (int i = 0; i < this->nPoints; i++) {
		this->transformed[i] = {
			(this->original[i].x * cosf(this->angle)) -
			(this->original[i].y * sinf(this->angle)) + this->position.x,
			(this->original[i].x * sinf(this->angle)) +
			(this->original[i].y * cosf(this->angle)) + this->position.y,
		};
	}
}

void PacMan::update(float dt,std::vector<Node*>& map_ref) {
	this->object_coll_rect->set_position(sf::Vector2f(this->position.x - this->radius, this->position.y - this->radius));
	this->prev_position = this->position;
	this->my_node = map_ref[(int)(this->position.y / TILESIZE) * MAP_WIDTH + (int)(this->position.x / TILESIZE)];
	//std::cout << my_node->position.x<<" "<< my_node->position.y << std::endl;

	this->get_keys();
	this->update_verticies();
	this->move_jaws();
	this->animate();
	this->move(dt);
	this->wrap_around();
	this->rect->update(this->position);
	this->coll_rect->set_position(sf::Vector2f(this->position.x - this->radius * 3, 
		this->position.y - this->radius * 3));
	
	

}
void PacMan::update_on_position(sf::Vector2f vec) {
	//this->rect->update(vec);
	this->set_position(vec);
	this->update_verticies();
	this->coll_rect->set_position(sf::Vector2f(this->position.x - this->radius * 3,
		this->position.y - this->radius * 3));
	
	
}

void PacMan::draw(sf::RenderWindow& i_window) {
	sf::ConvexShape polygon;

	polygon.setPointCount(this->drawable.size());
	for (int i = 0; i < this->drawable.size(); i++)
	{
		polygon.setPoint(i, this->drawable[i] );
	}
	polygon.setFillColor(this->color);
	
	
	i_window.draw(polygon);
	//this->rect->draw(i_window);
	
	
}

