#ifndef GAME_H
#define GAME_H

#include <random>
#include <thread>
#include "SDL.h"
#include <condition_variable>
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t screen_width, std::size_t screen_height, std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;


 private:
  Snake snake;
  SDL_Point food;
  SDL_Point bonus_food;
  int gameLevel = 1;
  bool bonus_food_placed = true;


  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::thread bonusFoodThread;
  std::condition_variable conditon_var;

  int score{0};

  void ResetBonusFood();
  void PlaceBonusFood();
  void BonusFoodTimer();
  void PlaceFood();
  void Update();
};

#endif