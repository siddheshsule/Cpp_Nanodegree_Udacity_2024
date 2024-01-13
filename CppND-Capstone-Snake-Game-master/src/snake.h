#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"
#include <memory>

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int screen_width, int screen_height, int grid_width, int grid_height)
      : screen_width(screen_width), screen_height(screen_height), grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2){}

  Snake();
  Snake (const Snake &other); // copy constructor
  Snake (Snake &&other) noexcept; // move constructor
  Snake& operator=(const Snake &other); // Copy assignment operator
  Snake& operator=(Snake &&other) noexcept; // move assignment operator
  void Update();

  void GrowBody();
  bool SnakeCell(int x, int y) const;

  Direction direction = Direction::kUp;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<std::unique_ptr<SDL_Point>> body;

 private:
  void UpdateHead();
  void UpdateBody(const SDL_Point &current_cell, const SDL_Point &prev_cell);  
  bool growing{false};
  int grid_width;
  int grid_height;
  int screen_width;
  int screen_height;

};

#endif