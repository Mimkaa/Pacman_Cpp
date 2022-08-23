#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>



class Polygon
{
private:
	unsigned int nPoints;
	std::vector<sf::Vector2<float>> original;
	std::vector<sf::Vector2<float>> transformed;
	sf::Vector2<float> position;
	float angle = 0.f;


	
public:
	unsigned int radius;
	Polygon(unsigned int Points, unsigned int rad, sf::Vector2<float> pos, float ang);
	virtual ~Polygon();
	// Accessors
	const sf::Vector2f get_position() const;
	const float get_angle() const;
	const std::vector<sf::Vector2<float>> get_t_points() const;
	void set_position(sf::Vector2<float>pos);
	void add_position(sf::Vector2<float>vec);
	//Functions
	void rotate(float dAngle);
	void update(sf::Vector2<float>);
	void update_verticies();
	void createPoints();
	void draw(sf::RenderWindow& i_window);


};

