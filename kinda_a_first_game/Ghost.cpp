#include "Ghost.hpp"
#include <iostream>
#include <cmath>
#include <random>

Ghost::Ghost(int rad, sf::Color col, float sc, Node* node, Node* corner, Node* original_pos) {
	this->radius = rad;
	this->color = col;
	this->tail_angle = 0.f;
	this->scale = sc;
	this->eye_size = this->radius / 2 * this->scale;
	this->right_eye_center = this->position - sf::Vector2f(-this->radius / 2 * this->scale, this->radius / 2 * this->scale);
	this->left_eye_center = this->position - sf::Vector2f(this->radius / 2 * this->scale, this->radius / 2 * this->scale);
	this->my_node = node;
	this->position = sf::Vector2f((float)(this->my_node->position.x * TILESIZE) + TILESIZE / 2, (float)(this->my_node->position.y * TILESIZE) + TILESIZE / 2);
	this->tail_position = this->position + sf::Vector2f(-this->radius, radius * 2);
	this->my_chase_target = nullptr;
	this->my_corner = corner;
	this->FoodToChase = 0;
	this->fleeing = false;
	this->scatter = true;
	this->chase = false;
	this->flee_target_set = false;
	this->timer_frightened = new Timer();
	this->timer_scatter = new Timer();
	this->timer_chase = new Timer();
	this->color_backup = col;
	this->scatter_timer_set = false;
	this->chase_timer_set = false;
	this->coll_rect = new SimpleRect(this->position.x - this->radius*this->scale, this->position.y - this->radius * this->scale, this->radius * 2 * this->scale, this->radius * 2 * this->scale);
	this->original_pos = original_pos;
	this->eaten = false;
	this->currentEatenFood = 0;
	this->begin_movement = new Timer();
	this->seconds_wait = 0.f;
}

void Ghost::get_not_wall(Node*& my_chase_target, Node* current_wall, std::list<Node*>& checked_walls) {
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

Ghost::~Ghost() {
	delete this->timer_frightened;
	delete this->timer_chase;
	delete this->timer_scatter;
	delete this->coll_rect;
	delete this->begin_movement;
}

void Ghost::set_food_limit(int limit) {
	this->FoodToChase = limit;
}

void Ghost::set_seconds_wait(float seconds) {
	this->seconds_wait = seconds;
}

float Ghost::wave_function(float x) {
	return abs(sinf(x));
}

void Ghost::reset_my_node(std::vector<Node*>& map_ref) {
	int y = this->position.y / TILESIZE;
	int x = this->position.x / TILESIZE;
	this->my_node = map_ref[y * MAP_WIDTH + x];
	this->position = sf::Vector2f((float)(this->my_node->position.x * TILESIZE) + TILESIZE / 2, (float)(this->my_node->position.y * TILESIZE) + TILESIZE / 2);
}

void Ghost::get_chase_target(PacMan& pacman, std::vector<Node*>& map_ref) {
	this->my_chase_target = map_ref[(int)(pacman.position.y / TILESIZE) * MAP_WIDTH + (int)(pacman.position.x / TILESIZE)];
}

void Ghost::follow_path(float dt, std::vector<Node*>& map_ref) {
	auto distance = [](sf::Vector2f a, sf::Vector2f b)
	{
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	};

	if (this->path.size() > 1 && !this->finished_segment) {
		sf::Vector2f end = sf::Vector2f((float)this->path[1].x, (float)this->path[1].y);
		sf::Vector2f start = sf::Vector2f((float)this->path[0].x, (float)this->path[0].y);
		sf::Vector2f dir_vec = end - start;
		float dir_vec_length = sqrtf(dir_vec.x * dir_vec.x + dir_vec.y * dir_vec.y);
		dir_vec.x /= dir_vec_length;
		dir_vec.y /= dir_vec_length;
		int speed = GHOSTSPEED;
		if (this->eaten) {
			speed = GHOSTSPEEDEATEN;
		}
		this->position += sf::Vector2f(dir_vec.x * speed * dt, dir_vec.y * speed * dt);
		//update target
		sf::Vector2f offset = sf::Vector2f(dir_vec.x * TILESIZE * 2, dir_vec.y * TILESIZE * 2);
		this->target = end + offset;
		// pop the last element of the path when we crossed a section of it
		if (distance(start, this->position) > distance(start, end)) {
			this->path.erase(this->path.begin());
			this->my_node = map_ref[(int)(this->position.y / TILESIZE) * MAP_WIDTH + (int)(this->position.x / TILESIZE)];
			sf::Vector2f new_pos = sf::Vector2f((float)this->path[0].x, (float)this->path[0].y);
			this->position = new_pos;
			this->finished_segment = true;

		}
	}
}


void Ghost::get_flee_target(PacMan& pacman, std::vector<Node*>& map_ref) {
	auto distance = [](Node* a, Node* b)
	{
		return sqrtf((a->position.x - b->position.x) * (a->position.x - b->position.x) + (a->position.y - b->position.y) * (a->position.y - b->position.y));
	};
	if (!this->flee_target_set) {
		if (distance(my_node, pacman.my_node) > 9) {
			std::list<Node*> NotInRadius;
			for (auto node : map_ref) {
				sf::Vector2f node_map_pos = sf::Vector2f((float)node->position.x * TILESIZE, (float)node->position.y * TILESIZE);
				if (distance(node, pacman.my_node) > 9 && !node->wall) {
					NotInRadius.push_back(node);

				}
			}
			// get a random node
			if (NotInRadius.size() > 0) {
				auto it = NotInRadius.cbegin();
				int random = rand() % NotInRadius.size();
				while (random--) {
					++it;
				}
				this->my_chase_target = *it;
			}
		}
		else {
			float x = floor(cosf(pacman.angle) + 0.5);
			float y = floor(sinf(pacman.angle) + 0.5);
			sf::Vector2f targer_vector_pacman = pacman.position ;
			sf::Vector2f my_map_pos = static_cast<sf::Vector2f>(this->my_node->position);
			my_map_pos.x *= TILESIZE;
			my_map_pos.y *= TILESIZE;
			sf::Vector2f pacman_me_vector = my_map_pos - targer_vector_pacman ;
			pacman_me_vector.x *= 2;
			pacman_me_vector.y *= 2;
			sf::Vector2f targer_vector = this->position + pacman_me_vector;
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
		this->flee_target_set = true;
	}
	// reset fleeing target
	if (distance(this->my_node, this->my_chase_target) < 3 || distance(this->my_node, pacman.my_node) < 5) {
		this->flee_target_set = false;
	}
}

void Ghost::update(float dt, std::vector<Node*>& map_ref) {
	int y = this->position.y / TILESIZE;
	int x = this->position.x / TILESIZE;
	this->my_node = map_ref[y * MAP_WIDTH + x];

	this->coll_rect->set_position(sf::Vector2f(this->position.x - this->radius*this->scale, this->position.y - this->radius * this->scale));
	// cancell being eaten
	if (this->eaten && this->my_node == this->original_pos) {
		this->eaten = false;
	}
	// Scatter -> Chase interchange
	if (this->scatter && ! this->scatter_timer_set) {
		this->timer_scatter->start();
		this->scatter_timer_set = true;
	}
	if (this->timer_scatter->exceeded(5)) {
		this->scatter = false;
		this->chase = true;
		this->scatter_timer_set = false;
	}

	if (this->chase && !this->chase_timer_set) {
		this->timer_chase->start();
		this->chase_timer_set = true;
	}
	if (this->timer_chase->exceeded(20)) {
		this->scatter = true;
		this->chase = false;
		this->chase_timer_set = false;
	}

	//Frightened
	if (this->fleeing) {
		this->color = sf::Color(0, 0, 255, 255);
		
	}
	else {
		this->color = this->color_backup;
	}
	if (this->timer_frightened->exceeded(4)) {
		this->fleeing = false;
	}

	// update tail
	this->tail_position = this->position + sf::Vector2f(-this->radius * this->scale, radius * this->scale / 2);
	this->tail_angle += 0.1;
	if (this->tail_angle > PI * 2) {
		this->tail_angle = 0.f;
	}
	this->wavy_bottom.clear();
	this->create_tail(this->tail_angle, this->radius * 2.01, 1, this->scale, this->scale, this->wavy_bottom, this->color);

	sf::Color color_rear(this->color.r / 2, this->color.g / 2, this->color.b / 2, this->color.a);
	this->wavy_bottom_rear.clear();
	this->create_tail(-this->tail_angle, this->radius * 2.01, 1, this->scale, this->scale, this->wavy_bottom_rear, color_rear);

	// update eyes position
	this->right_eye_center = this->position - sf::Vector2f((-this->radius / 2 * this->scale) * 1.2, this->radius / 2 * this->scale);
	this->left_eye_center = this->position - sf::Vector2f((this->radius / 2 * this->scale) * 1.2, this->radius / 2 * this->scale);



}
void Ghost::update_right_iris(sf::RenderWindow& i_window, sf::Vector2f target) {
	sf::Vector2f to_mouse = target - this->right_eye_center;
	float angle = atan2f(to_mouse.y, to_mouse.x);

	sf::Vector2f position_on_eye = sf::Vector2f(cosf(angle) * this->radius / 2 * this->scale, sinf(angle) * this->radius / 2 * this->scale);
	this->right_iris_pos = this->right_eye_center + position_on_eye;

	sf::Vector2f budge_back = position_on_eye;
	budge_back.x *= 0.5;
	budge_back.y *= 0.5;
	this->right_iris_pos -= budge_back;
}

void Ghost::update_left_iris(sf::RenderWindow& i_window, sf::Vector2f target) {
	sf::Vector2f to_mouse = target - this->left_eye_center;
	float angle = atan2f(to_mouse.y, to_mouse.x);

	sf::Vector2f position_on_eye = sf::Vector2f(cosf(angle) * this->radius / 2 * this->scale, sinf(angle) * this->radius / 2 * this->scale);
	this->left_iris_pos = this->left_eye_center + position_on_eye;

	sf::Vector2f budge_back = position_on_eye;
	budge_back.x *= 0.5;
	budge_back.y *= 0.5;
	this->left_iris_pos -= budge_back;
}

void Ghost::draw(sf::RenderWindow& i_window) {

	// update target
	/*sf::Vector2i mouse_position_int = sf::Mouse::getPosition(i_window);
	this->target = sf::Vector2f((float)mouse_position_int.x, (float)mouse_position_int.y);*/

	if (!this->eaten) {
		//head
		this->draw_head(i_window);
		//body
		this->draw_body(i_window);
		//tail 
		this->draw_tail(i_window);
	}
	// eyes 
	this->draw_eyes(i_window, this->target);
	/*if (this->my_chase_target != nullptr) {
		sf::RectangleShape target_rectangle;
		target_rectangle.setSize(sf::Vector2f(TILESIZE / 2, TILESIZE / 2));
		target_rectangle.setFillColor(this->color);
		target_rectangle.setPosition(this->my_chase_target->position.x * TILESIZE, this->my_chase_target->position.y * TILESIZE);
		i_window.draw(target_rectangle);
	}*/
	//----draw corners----
	/*sf::RectangleShape corner_rectangle;
	corner_rectangle.setSize(sf::Vector2f(TILESIZE / 2, TILESIZE / 2));
	corner_rectangle.setFillColor(this->color);
	corner_rectangle.setPosition(this->my_corner->position.x * TILESIZE, this->my_corner->position.y * TILESIZE);
	i_window.draw(corner_rectangle);*/
	

}

void Ghost::draw_head(sf::RenderWindow& i_window) {
	float delta_angle = (PI + 0.06) / 20;
	std::vector<sf::Vector2f> semi_circle;
	for (int i = 0; i < 20; i++) {
		sf::Vector2f point = -sf::Vector2f(cosf(i * delta_angle) * this->radius * this->scale, sinf(i * delta_angle) * this->radius * this->scale);
		point += sf::Vector2f(0, (-this->radius * this->scale / 2) * 0.8);
		point += this->position;
		semi_circle.push_back(point);
	}
	sf::ConvexShape semi_circle_polygon;
	semi_circle_polygon.setPointCount(20);
	for (int i = 0; i < 20; i++) {
		semi_circle_polygon.setPoint(i, semi_circle[i]);
	}
	semi_circle_polygon.setOutlineColor(this->color);
	semi_circle_polygon.setFillColor(this->color);
	//semi_circle_polygon.setOutlineThickness(1);

	i_window.draw(semi_circle_polygon);
}

void Ghost::draw_body(sf::RenderWindow& i_window) {
	sf::VertexArray body_quad(sf::Quads, 4);
	body_quad[0].position = this->position - sf::Vector2f(this->radius * this->scale, this->radius * this->scale / 2);
	body_quad[1].position = this->position - sf::Vector2f(-this->radius * this->scale, this->radius * this->scale / 2);
	body_quad[2].position = this->position - sf::Vector2f(-this->radius * this->scale, -this->radius * this->scale / 2);
	body_quad[3].position = this->position - sf::Vector2f(this->radius * this->scale, -this->radius * this->scale / 2);
	body_quad[0].color = this->color;
	body_quad[1].color = this->color;
	body_quad[2].color = this->color;
	body_quad[3].color = this->color;
	i_window.draw(body_quad);
}

void Ghost::draw_eyes(sf::RenderWindow& i_window, sf::Vector2f target) {
	sf::CircleShape left_eye;
	left_eye.setRadius(this->radius / 2 * this->scale);
	left_eye.setFillColor(sf::Color(255, 255, 255, 255));
	left_eye.setPosition(this->right_eye_center - sf::Vector2f(this->eye_size, this->eye_size));
	i_window.draw(left_eye);

	sf::CircleShape right_eye;
	left_eye.setRadius(this->radius / 2 * this->scale);
	left_eye.setFillColor(sf::Color(255, 255, 255, 255));
	left_eye.setPosition(this->left_eye_center - sf::Vector2f(this->eye_size, this->eye_size));
	i_window.draw(left_eye);
	// ______update irises__________

	//draw right_iris

	this->update_right_iris(i_window, target);
	sf::RectangleShape right_iris_rect;
	right_iris_rect.setSize(sf::Vector2f(this->radius / 2 * this->scale, this->radius / 2 * this->scale));
	right_iris_rect.setFillColor(sf::Color(0, 0, 255, 255));
	right_iris_rect.setPosition(this->right_iris_pos - sf::Vector2f(this->eye_size / 2, this->eye_size / 2));
	i_window.draw(right_iris_rect);

	//draw left_iris
	this->update_left_iris(i_window, target);
	sf::RectangleShape left_iris_rect;
	left_iris_rect.setSize(sf::Vector2f(this->radius / 2 * this->scale, this->radius / 2 * this->scale));
	left_iris_rect.setFillColor(sf::Color(0, 0, 255, 255));
	left_iris_rect.setPosition(this->left_iris_pos - sf::Vector2f(this->eye_size / 2, this->eye_size / 2));
	i_window.draw(left_iris_rect);
}

void Ghost::draw_tail(sf::RenderWindow& i_window) {
	//tail rear
	for (int i = 0; i < this->wavy_bottom_rear.size(); i++) {
		for (auto& element : this->wavy_bottom_rear[i]) {
			i_window.draw(element);
		}

	}

	//tail
	for (int i = 0; i < this->wavy_bottom.size(); i++) {
		for (auto& element : this->wavy_bottom[i]) {
			i_window.draw(element);
		}

	}
}

void Ghost::create_tail(float begin, float rad, float step, float x_scale, float y_scale, std::vector<std::vector<sf::VertexArray>>& vector, sf::Color color) {
	//there we just create a rectangle between previous and next point of the function graph
	//using triangles

	for (float i = begin; i < begin + rad; i += step) {
		sf::VertexArray triangle1(sf::Triangles, 3);
		// define the position of the triangle's points
		triangle1[0].position = sf::Vector2f(i * x_scale, this->wave_function(i) * y_scale) + this->tail_position - sf::Vector2f(begin * x_scale, 0);
		triangle1[1].position = sf::Vector2f(triangle1[0].position.x, triangle1[0].position.y - (triangle1[0].position.y - this->tail_position.y));
		triangle1[2].position = vector.empty() ? this->tail_position : vector[vector.size() - 1][0][0].position;

		// define the color of the triangle's points
		triangle1[0].color = color;
		triangle1[1].color = color;
		triangle1[2].color = color;


		sf::VertexArray triangle2(sf::Triangles, 3);
		triangle2[0].position = triangle1[1].position;
		triangle2[1].position = vector.empty() ? this->tail_position : vector[vector.size() - 1][0][1].position;
		triangle2[2].position = vector.empty() ? this->tail_position : vector[vector.size() - 1][0][0].position;

		triangle2[0].color = color;
		triangle2[1].color = color;
		triangle2[2].color = color;

		std::vector<sf::VertexArray> quad;
		quad.push_back(triangle1);
		quad.push_back(triangle2);
		vector.push_back(quad);

	}


}


