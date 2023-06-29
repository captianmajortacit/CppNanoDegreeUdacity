#include "game.h"
#include <iostream>
#include <string>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : score(0),
      snake(grid_width, grid_height, this->checkForLevels()),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

int Game::checkForInt()
{
  int loc_retVal;
  std::string loc_userInput;
  std::cout<<"Set Difficulty Level:"<<std::endl<<"1.Easy"<<std::endl<<"2.Medium"<<std::endl<<"3.Hard"<<std::endl<<"Enter your choice:";
  std::getline(std::cin, loc_userInput);
  try{
    loc_retVal = std::stoi(loc_userInput);
  }
  catch(const std::exception& e){
    std::cerr<<e.what()<<std::endl;
    loc_retVal = 65535;
  }
  return loc_retVal;
}
int Game::checkForLevels(){
  
  int loc_retVal = this->checkForInt();
  while((loc_retVal > 3) || (loc_retVal < 1) || (loc_retVal == 65535)){
    if(loc_retVal == 65535){
      std::cout<<"Enter a value between 1 and 3"<<std::endl;
    }
    else if(loc_retVal > 3){
      std::cout<<"The entered level is more than 3"<<std::endl;
    }
    else if(loc_retVal < 1){
      std::cout<<"The entered level is less than 1"<<std::endl;
      
    }
    else{
      //do nothing
    }
    loc_retVal = this->checkForInt();
  }
  return loc_retVal;
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
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
  _player.Cyclic(score,snake.Get_updateSpeedFlag());
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
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
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    if(snake.Get_updateSpeedFlag() == 3)
    {
      snake.speed += 0.02;
    }
    else if(snake.Get_updateSpeedFlag() == 2)
    {
      snake.speed += 0.01;
    }
    else 
    {
      //do nothing the speed should remain the same
    }
    
  }
}