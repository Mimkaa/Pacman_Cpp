#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Polygon.hpp"
#include "SimpleRect.hpp"
#include "Node.hpp"

class PacMan
{
private:

	unsigned int nPoints;
	std::vector<sf::Vector2<float>> original;
	std::vector<sf::Vector2<float>> transformed;
	int nom_dir;
	int last_update;
	int current_time;
	sf::Color color;
	Polygon*rect;
	
	
	

public:
	int UpperJawInd;
	int LowerJawInd;
	std::vector<sf::Vector2<float>> drawable;
	int eaten;
	int score;
	sf::Vector2<float> prev_position;
	Node* my_node;
	sf::Vector2f original_pos;
	SimpleRect* coll_rect;
	SimpleRect* object_coll_rect;
	unsigned int radius;
	float angle;
	sf::Vector2<float> position;
	// Constructor, Destructor
	PacMan(sf::Vector2<float> pos, int vec_UpperJaw, 
		int vec_LowerJaw, unsigned int numPoints, unsigned int rad, sf::Color col, float ang);
	virtual ~PacMan();

	// Getters and setters
	void set_position(sf::Vector2<float> vec);
	const sf::Vector2<float> get_position() const;
	Polygon* get_rect() const;
	void add_position(sf::Vector2<float> vec);

	//Functions
	void update(float dt, std::vector<Node*>& map_ref);
	void draw(sf::RenderWindow& i_window);
	void createPoints();
	void animate();
	void get_keys();
	bool FloatComp(float a, float b, float epsilon);
	void move(float dt);
	void move_jaws();
	void wrap_around();
	void initialize_rects();
	void update_verticies();
	void update_on_position(sf::Vector2f vec);




};

