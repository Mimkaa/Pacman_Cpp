#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.hpp"
#include "PacMan.hpp"
#include <array>
#include "Settings.hpp"
#include "Polygon.hpp"
#include "Node.hpp"
#include "Ghost.hpp"
#include "Blinky.hpp"
#include "Pinky.hpp"
#include "Inky.hpp"
#include <list>
#include "Food.hpp"
#include "Clyde.hpp"
#include "Energizer.hpp"
#include "Timer.hpp"

class Game
{
private:
	//variables
	std::vector<Node*> map;
	std::list<Food*> food_list;
	std::list<Energizer*> energizer_list;
	PacMan* pacman;
	Blinky* Redg;
	Pinky* Pinkg;
	Inky* Cyang;
	Clyde* Yellowg;
	int consequitive_eats;
	std::vector<Ghost*> ghosts;
	std::array<std::string, MAP_HEIGHT> map_sketch;
	std::array<std::array<CELL, MAP_HEIGHT>, MAP_WIDTH> converted_sketch;
	std::vector<Polygon>walls;
	float current_fps;
	// Window
	sf::RenderWindow* window;
	sf::Event ev;
	sf::VideoMode videoMode;
	sf::Font font;
	bool start_game_question;
	bool game_over;
	int lives;
	bool won;
	Timer* victory_blinking;
	bool blink ;
	sf::Color current_wall_color;
	
	// privat functions
	void initializeVariables();
	void initWindow();
public:
	//Constructor
	Game();

	// Destructor
	virtual ~Game();

	// Accessors
	const bool running() const;

	//functions
	void convert_sketch();
	void draw_map();
	void events();
	void update(float dt);
	void draw();
	int len();
	void initializeMap();

	
};


