#pragma once
#include <SFML/Graphics.hpp>
class SimpleRect {
private:
	float x;
	float y;
	int width;
	int height;
public:

	const float get_x() const;
	const float get_y() const;
	const int get_width() const;
	const int get_height() const;

	SimpleRect(float v_x, float v_y, int v_width, int v_height);
	void set_position(sf::Vector2f pos);
	virtual ~SimpleRect();
	bool contains(sf::Vector2f vec);
	bool collide_rect(SimpleRect& rect);
};