#pragma once
#include <SFML/Graphics.hpp>
#include "Settings.hpp"
#include "Node.hpp"
#include "AStartPathFinding.hpp"
#include "PacMan.hpp"
#include "Timer.hpp"
#include <list>
#include "SimpleRect.hpp"
#include "Timer.hpp"
class Ghost
{

public:
	int radius;
	sf::Color color;
	std::vector<sf::Vector2<float>> original;
	std::vector<sf::Vector2<float>> transformed;
	std::vector<std::vector<sf::VertexArray>> wavy_bottom;
	std::vector<std::vector<sf::VertexArray>> wavy_bottom_rear;
	float tail_angle;
	float scale;
	sf::Vector2f tail_position;
	sf::Vector2f left_eye_center;
	sf::Vector2f left_iris_pos;
	sf::Vector2f right_eye_center;
	sf::Vector2f right_iris_pos;
	int eye_size;
	sf::Vector2f target;
	sf::Vector2f position;
	std::vector <sf::Vector2i> path;
	Node* my_node;
	Node* my_chase_target;
	bool finished_segment = true;
	Node* my_corner;
	Node* original_pos;
	int FoodToChase;
	bool fleeing ;
	bool scatter;
	bool chase;
	bool flee_target_set;
	Timer* timer_frightened;
	Timer* timer_scatter;
	bool scatter_timer_set;
	Timer* timer_chase;
	bool chase_timer_set;
	sf::Color color_backup;
	SimpleRect* coll_rect;
	bool eaten;
	int currentEatenFood;
	Timer* begin_movement;
	float seconds_wait;



	Ghost(int rad, sf::Color col, float sc, Node* node, Node* corner, Node* original_pos);
	virtual ~Ghost();
	float wave_function(float x);
	void update(float dt, std::vector<Node*>& map_ref);
	void draw(sf::RenderWindow& i_window);
	void draw_head(sf::RenderWindow& i_window);
	void draw_body(sf::RenderWindow& i_window);
	void draw_tail(sf::RenderWindow& i_window);
	void draw_eyes(sf::RenderWindow& i_window, sf::Vector2f target);
	void create_tail(float begin, float rad, float step, float x_scale, float y_scale, std::vector<std::vector<sf::VertexArray>>& vector, sf::Color color);
	void update_right_iris(sf::RenderWindow& i_window, sf::Vector2f target);
	void update_left_iris(sf::RenderWindow& i_window, sf::Vector2f target);
	void follow_path(float dt, std::vector<Node*>& map_ref);
	void reset_my_node(std::vector<Node*>& map_ref);
	virtual void get_chase_target(PacMan& pacman, std::vector<Node*>& map_ref);
	void set_food_limit(int limit);
	void get_flee_target(PacMan& pacman, std::vector<Node*>& map_ref);
	void get_not_wall(Node*& my_chase_target, Node* current_wall, std::list<Node*>& checked_walls);
	void set_seconds_wait(float seconds);
};

