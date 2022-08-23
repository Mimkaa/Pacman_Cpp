#pragma once
constexpr int TILESIZE = 32;
constexpr int MAP_WIDTH = 19;
constexpr int MAP_HEIGHT = 21;
constexpr short int  FRAME_DURATION = 16667;
constexpr float  PACMANSPEED = 100.f;
constexpr float  GHOSTSPEED = 50.f;
constexpr float  GHOSTSPEEDEATEN = 220.f;
constexpr int SCREEN_WIDTH = MAP_WIDTH * TILESIZE;
constexpr int SCREEN_HEIGHT = MAP_HEIGHT * TILESIZE;
enum CELL {
	door,
	empty,
	food,
	wall,
	energizer
};
const double PI = 3.141592653589793238463;
constexpr int BLINKY_NEEDED_SECONDS = 0;
constexpr int INKY_NEEDED_SECONDS = 10;
constexpr int PINKY_NEEDED_SECONDS = 3;
constexpr int CLYDE_NEEDED_SECONDS = 20;
