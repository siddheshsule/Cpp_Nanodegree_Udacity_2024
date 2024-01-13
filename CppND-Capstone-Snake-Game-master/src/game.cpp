#include "game.h"
#include <iostream>
#include "SDL.h"
#include <thread>
#include <mutex>

std::mutex mtx;

Game::Game(std::size_t screen_width, std::size_t screen_height, std::size_t grid_width, std::size_t grid_height)
    : snake(screen_width, screen_height, grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  // Introducing thread initialization
  std::thread input_thread([&] {
    while (running) {
      controller.HandleInput(running, snake);
    }
  });


  while (running) {
    frame_start = SDL_GetTicks();
    Update();

    // Apply lock before rendering to avoid data races
    std::lock_guard<std::mutex> lock(mtx);
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, gameLevel ,frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }

  input_thread.join();
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.

    std::lock_guard<std::mutex> lock(mtx);
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  //std::lock_guard<std::mutex> lock(mtx); // This lock hangs the game!
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();

    if (snake.size % 10 == 5 || snake.size % 10 == 0) {
      snake.speed += 0.05;
      gameLevel++;
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }



