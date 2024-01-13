#include "snake.h"
#include <cmath>
#include <iostream>
#include <memory>

Snake::Snake() : body() {}

Snake::Snake(const Snake &other) : body(other.body.size()) {
  //Copy constructor
  for (size_t i = 0; i < other.body.size(); ++i) {
    body[i] = std::make_unique<SDL_Point>(*other.body[i]);
  }
}

Snake::Snake(Snake &&other) noexcept : body(std::move(other.body)) {
  // Move constructor
}

Snake& Snake::operator=(const Snake &other) {
  // Copy assignment
  if (this != &other) {
    body.resize(other.body.size());
    for (size_t i = 0; i < other.body.size(); ++i) {
      body[i] = std::make_unique<SDL_Point>(*other.body[i]);
    }
  }
  return *this;
}

Snake& Snake::operator=(Snake &&other) noexcept {
  // Move assignment operator
  if (this != &other) {
    body = std::move(other.body);
  }
  return *this;
}


void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(const SDL_Point &current_head_cell, const SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.emplace_back(std::make_unique<SDL_Point>(prev_head_cell));
  
 // Check if the snake collided with the wall
  for (auto const& item : body) {
    if ((current_head_cell.x == 0 || current_head_cell.x > screen_width) ||
      (current_head_cell.y == 0 || current_head_cell.y > screen_height)) {
      alive = false;
    }
  }


  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item->x && current_head_cell.y == item->y) {
      alive = false;
    }
  }

}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) const {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item->x && y == item->y) {
      return true;
    }
  }
  return false;
}

