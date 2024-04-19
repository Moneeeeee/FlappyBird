#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

class FlappyBird {

  std::shared_ptr<sf::RenderWindow> window;
  float gravity, frame, space;
  int count, score;
  sf::Texture bg, flappy, pipe;
  std::shared_ptr<sf::Sprite> background, bird, pipeBottom, pipeTop;
  std::vector<sf::Sprite> pipes;
  bool gameover, add;
  bool gameStarted;
  sf::Font font;
  sf::Text txt_score, txt_gameover;

  std::vector<std::string> leaderboardTexts;
  protected:
   void events();
   void draw();
   void game();
   void movePipes();
   void setAnimeBird();
   void moveBird();

  public:
   FlappyBird();
   void run();
};
