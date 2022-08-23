#include "Game.hpp"
#include "PacMan.hpp"
#include "Settings.hpp"
#include "Polygon.hpp"
#include <cmath>
#include "ShapeOverlap_DIAGS_STATIC.hpp"
#include <string>
#include <math.h>
#include <format>
#include <iostream>
#include<tuple>
#include <type_traits>


// privat
void Game::initializeVariables()
{
    this->window = nullptr;
    this->pacman = nullptr;
    this->Redg = nullptr;
    this->Yellowg = nullptr;
    this->Pinkg = nullptr;
    this->Cyang = nullptr;
    this->map_sketch = {
        "###################",
        "#........#........#",
        "#.##.###.#.###.##.#",
        "#.................#",
        "#.##.#.#####.#.##.#",
        "#.*..#...#...#..*.#",
        "####.###.#.###.####",
        "####.#...1...#.####",
        "####.#.##d##.#.####",
        ".......#234#.......",
        "####.#.#####.#.####",
        "####.#.......#.####",
        "####.#.#####.#.####",
        "#........#........#",
        "#.##.###.#.###.##.#",
        "#..#...........#..#",
        "##.#.#.#####.#.#.##",
        "#.*..#...#...#..*.#",
        "#.######.#.######.#",
        "#.................#",
        "###################",
        
    };
   
    this->converted_sketch = {};// initialize the array with 0s
    this->initializeMap();
    this->convert_sketch();
    // update all veretcies of the polygon
    for (int i = 0; i < this->walls.size(); i++) {
        this->walls[i].update(this->walls[i].get_position());
    }
    // font
    if (!this->font.loadFromFile("Resource/PixelatedRegular-aLKm.ttf"))
    {
        std::cout << "You are so dump that forgot to include your font before calling me;);););););)" << std::endl;
    }
    this->current_fps = 0.f;
    this->consequitive_eats = 1;
    this->start_game_question = false;
    this->game_over = false;
    this->lives = 3;
    this->won = false;
    this->victory_blinking = new Timer();
    this->blink = false;
    this->current_wall_color = sf::Color::Blue;
    
   
    

}
void Game::initializeMap() {
    // initialize map
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++)
            this->map.push_back(new Node(sf::Vector2i(j, i)));
    }
    // create neighbours
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            if (j < MAP_HEIGHT - 1) {
                this->map[j * MAP_WIDTH + i]->Neighbours.push_back(this->map[(j + 1) * MAP_WIDTH + i]);
            }
            if (j > 0) {
                this->map[j * MAP_WIDTH + i]->Neighbours.push_back(this->map[(j - 1) * MAP_WIDTH + i]);
            }
            if (i < MAP_WIDTH - 1) {
                this->map[j * MAP_WIDTH + i]->Neighbours.push_back(this->map[j * MAP_WIDTH + i + 1]);
            }
            if (i > 0) {
                this->map[j * MAP_WIDTH + i]->Neighbours.push_back(this->map[j * MAP_WIDTH + i - 1]);
            }


        }
    }
}

void Game::initWindow()
{
    this->videoMode.height = SCREEN_HEIGHT;
    this->videoMode.width = SCREEN_WIDTH;
    this->window = new sf::RenderWindow(this->videoMode, "PACMAN");
    // initialize Pacman
    this->pacman = new PacMan(sf::Vector2<float>(TILESIZE * 1.5, TILESIZE * 1.5), 0, 0, 30, 15, sf::Color(255, 255, 0, 255), 0.f);
    this->pacman->createPoints();
    
}
// public
// Constructor and Destructor
Game::Game()
{
    this->initializeVariables();
    this->initWindow();
}

Game::~Game()
{
    delete this->victory_blinking;
    delete this->window;
    delete this->pacman;
    for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) {
        delete this->map[i];
    }
    this->map.clear();
    delete this->Redg;
    delete this->Pinkg;
    delete this->Yellowg;
    delete this->Cyang;
    this->ghosts.clear();

    for (auto& food : this->food_list) {
        delete food;
    }
    this->food_list.clear();

    for (auto& en : this->energizer_list) {
        delete en;
    }
    this->energizer_list.clear();

}

//Accessors
const bool Game::running() const
{
    return this->window->isOpen();
}

//Functions
int Game::len() {
    return (sizeof(this->map_sketch) / sizeof(this->map_sketch[0]));
}
void Game::events()
{
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->start_game_question) {
                this->start_game_question = false;
                this->pacman->eaten = false;
            }
            else if (this->game_over || this->won) {
                this->window->close();
            }
            if (this->ev.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }

    }
}




void Game::update(float dt)
{
    this->current_fps = floor(1000000/(1000000 * dt)+0.5);
    this->events();
    // check if won
    if (this->food_list.empty() && this->energizer_list.empty()) {
        this->won = true;
        
    }

    // reset pacman and ghosts if pacman opened mouth completely and disappeared
    if (this->pacman->drawable.size() < 2 && this->pacman->eaten) {
        this->pacman->position = this->pacman->original_pos;
        for (auto ghost_inst : this->ghosts) {
            ghost_inst->currentEatenFood = 0;
            ghost_inst->my_node = ghost_inst->original_pos;
            ghost_inst->path.clear();
            ghost_inst->position = sf::Vector2f((float)(ghost_inst->my_node->position.x * TILESIZE) + TILESIZE / 2, (float)(ghost_inst->my_node->position.y * TILESIZE) + TILESIZE / 2);
            ghost_inst->update(dt, this->map);
            ghost_inst->tail_position = sf::Vector2f(-50, -50);
            ghost_inst->left_eye_center = sf::Vector2f(-50, -50);
            ghost_inst->right_eye_center = sf::Vector2f(-50, -50);
            ghost_inst->begin_movement->start();
        }
        this->pacman->LowerJawInd = 0;
        this->pacman->UpperJawInd = 0;
        if (this->lives > 0) {
            this->start_game_question = true;
        }
        else {
            this->game_over = true;
        }


    }
    if (!this->start_game_question && ! this->game_over && ! this->won) {
        this->pacman->update(dt, this->map);
    }
    

    if (!this->pacman->eaten && !this->won) {

        // get all poligons in visinity not to check collisions against all of the at once
        std::vector<Polygon> to_collide_with;
        for (auto& element : this->walls) {
            if (this->pacman->coll_rect->contains(element.get_position())) {
                to_collide_with.push_back(element);
            }
        }
        for (auto& element : to_collide_with) {

            ShapeOverlap_DIAGS_Resolve(*this->pacman->get_rect(), element);
        }
        // set the position of our pacman after the collision
        this->pacman->set_position(this->pacman->get_rect()->get_position());
        this->pacman->coll_rect->set_position(sf::Vector2f(this->pacman->position.x - this->pacman->radius * 3,
            this->pacman->position.y - this->pacman->radius * 3));

        //-----manage food-----
        std::list<Food*> food_list_delete;
        for (auto& food : this->food_list) {
            if (food->coll_rect->collide_rect(*this->pacman->object_coll_rect)) {
                food_list_delete.push_back(food);
                this->pacman->score += 10;
                this->Redg->currentEatenFood += 10;
                this->Pinkg->currentEatenFood += 10;
                this->Cyang->currentEatenFood += 10;
                this->Yellowg->currentEatenFood += 10;
            }

        }
        for (auto& food : food_list_delete) {
            delete food;
            food_list.remove(food);
        }

        // -----manage energizers-----
        std::list<Energizer*> energizer_list_delete;
        for (auto& en : this->energizer_list) {
            if (en->coll_rect->collide_rect(*this->pacman->object_coll_rect)) {
                energizer_list_delete.push_back(en);
                this->pacman->score += 50;
                this->Redg->currentEatenFood += 50;
                this->Pinkg->currentEatenFood += 50;
                this->Cyang->currentEatenFood += 50;
                this->Yellowg->currentEatenFood += 50;
                for (auto& ghost_inst : this->ghosts)
                {
                    ghost_inst->fleeing = true;
                    ghost_inst->timer_frightened->start();
                }

            }

        }
        for (auto& en : energizer_list_delete) {
            delete en;
            this->energizer_list.remove(en);
        }

        // -----update ghosts-----
    
        if (!this->Redg->fleeing) {
           
            this->consequitive_eats = 1;
        }
        for (auto& ghost_inst : this->ghosts) {

            if (ghost_inst->begin_movement->exceeded(ghost_inst->seconds_wait)) {
                // different behaviours
                if (ghost_inst->fleeing)
                {
                    ghost_inst->get_flee_target(*this->pacman, this->map);
                    // check if eaten
                    if (ghost_inst->coll_rect->collide_rect(*this->pacman->object_coll_rect)) {
                        ghost_inst->eaten = true;
                        ghost_inst->fleeing = false;
                        this->pacman->score += 400 * this->consequitive_eats;
                        this->consequitive_eats += 1;

                    }
                }
                else if (ghost_inst->eaten) {
                    ghost_inst->my_chase_target = ghost_inst->original_pos;

                }
                else if (ghost_inst->scatter) {
                    ghost_inst->my_chase_target = ghost_inst->my_corner;
                    // collide_with_pacman
                    if (ghost_inst->coll_rect->collide_rect(*this->pacman->object_coll_rect)) {
                        this->pacman->eaten = true;
                        this->lives -= 1;
                    }
                }
                else if (ghost_inst->chase)
                {
                    // collide_with_pacman
                    if (ghost_inst->coll_rect->collide_rect(*this->pacman->object_coll_rect)) {
                        this->pacman->eaten = true;
                        this->lives -= 1;
                    }
                    // get a target to chase
                    if (dynamic_cast<Inky*> (ghost_inst) == NULL) {
                        ghost_inst->get_chase_target(*this->pacman, this->map);
                    }
                    else {
                        this->Cyang->get_chase_target(*this->pacman, this->map, *this->Redg);
                    }
                }
                // regain chasing
                if (ghost_inst->path.size() < 2) { ghost_inst->finished_segment = true; }
                // create the path for our ghost to follow
                if (ghost_inst->finished_segment) {
                    AStarPathFinding(ghost_inst->my_node, ghost_inst->my_chase_target, this->map);
                    ghost_inst->path.clear();
                    ghost_inst->path.push_back(sf::Vector2i((ghost_inst->my_chase_target->position.x * TILESIZE) + TILESIZE / 2,
                        (ghost_inst->my_chase_target->position.y * TILESIZE) + TILESIZE / 2));
                    // create a path for the ghost
                    Node* current = ghost_inst->my_chase_target;
                    while (current->parent != nullptr) {
                        ghost_inst->path.push_back(sf::Vector2i((current->parent->position.x * TILESIZE) + TILESIZE / 2,
                            (current->parent->position.y * TILESIZE) + TILESIZE / 2));
                        current = current->parent;
                    }
                    std::reverse(ghost_inst->path.begin(), ghost_inst->path.end());
                    ghost_inst->reset_my_node(this->map);
                    ghost_inst->finished_segment = false;
                }
                ghost_inst->follow_path(dt, this->map);
                ghost_inst->update(dt, this->map);
            }

        }
    }
    
    
}

void Game::draw()
{
    
    this->window->clear(sf::Color(0, 0, 0, 255));
    /*sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(this->pacman->coll_rect->get_width(), this->pacman->coll_rect->get_height()));
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1);
    rectangle.setFillColor(sf::Color(0, 0, 0, 255));
    rectangle.setPosition(this->pacman->coll_rect->get_x(), this->pacman->coll_rect->get_y());
    this->window->draw(rectangle);*/

    //Draw game objects
    this->draw_map();
    if (!this->won) {
        this->pacman->draw(*this->window);
    }
    
    
    //draw walls
    /*for (int i = 0; i < this->walls.size(); i++) {
        this->walls[i].draw(*this->window);
    }*/

    // draw food
    for (auto&food : this->food_list) {
        
        food->draw(*this->window);
    }

    // draw energizers
    for (auto& en : this->energizer_list) {
        en->draw(*this->window);
    }

    // draw ghosts
    if (!this->pacman->eaten && !this->won) {
        for (auto& ghost : this->ghosts) {
            ghost->draw(*this->window);
        }
    }
    // draw lives
    sf::Text disp_lives;
    disp_lives.setFont(this->font);
    disp_lives.setCharacterSize(60);
    disp_lives.setFillColor(sf::Color::White);
    disp_lives.setString("Lives: ");
    disp_lives.setPosition(SCREEN_WIDTH - 140 - disp_lives.getLocalBounds().width, -30);
    this->window->draw(disp_lives);
    for (int i = 1; i < this->lives+1; i++) {

        sf::CircleShape circle;
        circle.setRadius(15);
        circle.setFillColor(sf::Color::Yellow);
        circle.setPosition(SCREEN_WIDTH - i * 40,0);
        this->window->draw(circle);
    }

    // -----draw text-----
    if (this->start_game_question) {
        sf::Text disp_ready;
        disp_ready.setFont(this->font);
        disp_ready.setCharacterSize(60);
        disp_ready.setFillColor(sf::Color::White);
        disp_ready.setString("Ready!(press any key to begin)");
        disp_ready.setPosition(SCREEN_WIDTH / 2 - disp_ready.getLocalBounds().width / 2, SCREEN_WIDTH / 2 - disp_ready.getLocalBounds().height / 2);
        this->window->draw(disp_ready);
    }

    if (this->game_over) {
        sf::Text disp_ready;
        disp_ready.setFont(this->font);
        disp_ready.setCharacterSize(60);
        disp_ready.setFillColor(sf::Color::Red);
        disp_ready.setString("Game Over");
        disp_ready.setPosition(SCREEN_WIDTH / 2 - disp_ready.getLocalBounds().width / 2, SCREEN_WIDTH / 2 - disp_ready.getLocalBounds().height / 2);
        this->window->draw(disp_ready);
    }
    // show score
    sf::Text disp_score;
    disp_score.setFont(this->font);
    disp_score.setCharacterSize(60);
    disp_score.setFillColor(sf::Color::White);
    disp_score.setPosition(5, -30);
    disp_score.setString("Score: " + std::to_string(this->pacman->score));
    this->window->draw(disp_score);
    
    this->window->display();
}

void Game::draw_map() {
    if (this->won) {
        sf::Color color1 = sf::Color::Blue;
        sf::Color color2 = sf::Color::White;
        if (this->victory_blinking->exceeded(0.25)) {
            this->blink = !this->blink;
            this->victory_blinking->start();
        }
        if (this->blink) {
            this->current_wall_color = color1;
        }
        else {
            this->current_wall_color = color2;
        }
    }
    for (int i = 0; i < this->map.size(); i++) {
        if (this->map[i]->wall) {
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(TILESIZE, TILESIZE));
            rectangle.setFillColor(this->current_wall_color);
            rectangle.setPosition(this->map[i]->position.x * TILESIZE, this->map[i]->position.y * TILESIZE);
            this->window->draw(rectangle);
        }
    }
}

void Game::convert_sketch() {
    
    for (int i = 0; i < MAP_HEIGHT ; i++) {
        for (int j = 0; j < MAP_WIDTH ; j++) {
            switch (this->map_sketch[i][j]) {
                case '#':
                {
                    this->converted_sketch[j][i] = CELL::wall;
                    // create walls
                    this->walls.push_back(Polygon(4, (int)(TILESIZE/ sqrt(2)), sf::Vector2f(j* TILESIZE + TILESIZE/2, i* TILESIZE + TILESIZE / 2), PI/4));
                    this->map[i * MAP_WIDTH + j]->wall = true;
                    break;
                }
                case '.': 
                {
                    this->converted_sketch[j][i] = CELL::empty;
                    Food* new_food = new Food(sf::Color(255, 255, 0, 255), sf::Vector2f(j * (TILESIZE )+ TILESIZE/2, i * (TILESIZE )+TILESIZE / 2), 3);
                    this->food_list.push_back(new_food);
                    break;
                }
                case '*':
                {
                    Energizer* new_energizer = new Energizer(sf::Color(255, 255, 0, 255), sf::Vector2f(j * (TILESIZE)+TILESIZE / 2, i * (TILESIZE)+TILESIZE / 2), 3);
                    this->energizer_list.push_back(new_energizer);
                    break;
                }
                case '1':
                {
                    this->Redg = new Blinky(5, sf::Color(255, 0, 0, 255), 3.f, this->map[i * MAP_WIDTH + j], this->map[1 * MAP_WIDTH + MAP_WIDTH-2], this->map[i * MAP_WIDTH + j]);
                    this->ghosts.push_back(this->Redg);
                    this->Redg->begin_movement->start();
                    break;
                }
                case '2':
                {
                    this->Cyang = new Inky(5, sf::Color(0, 255, 255, 255), 3.f, this->map[i * MAP_WIDTH + j], this->map[(MAP_HEIGHT-2) * MAP_WIDTH + MAP_WIDTH - 2], this->map[i * MAP_WIDTH + j]);
                    this->ghosts.push_back(this->Cyang);
                    this->Cyang->begin_movement->start();
                    break;
                }
                case '3':
                {
                    this->Pinkg = new Pinky(5, sf::Color(255, 192, 203, 255), 3.f, this->map[i * MAP_WIDTH + j], this->map[1 * MAP_WIDTH +1], this->map[i * MAP_WIDTH + j]);
                    this->ghosts.push_back(this->Pinkg);
                    this->Pinkg->begin_movement->start();
                    break;
                }
                case '4':
                {
                    this->Yellowg = new Clyde(5, sf::Color(255, 165, 0, 255), 3.f, this->map[i * MAP_WIDTH + j], this->map[(MAP_HEIGHT - 2) * MAP_WIDTH + 1], this->map[i * MAP_WIDTH + j]);
                    this->ghosts.push_back(this->Yellowg);
                    this->Yellowg->begin_movement->start();
                    break;
                }

            }

        }
    }
}
