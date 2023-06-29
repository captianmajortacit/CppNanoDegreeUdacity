#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height, int difficultyLevel)
      : speed(0.1f),
        alive(true),
        size(1),
        updateSpeedFlag(difficultyLevel),
        grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2)
        {}

  void Update();

  void Set_updateSpeedFlag(const int arg);
  int  Get_updateSpeedFlag() const;
  void GrowBody();
  bool SnakeCell(int x, int y);

  Direction direction = Direction::kUp;

  float speed;
  int updateSpeedFlag;
  int size;
  bool alive;
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif