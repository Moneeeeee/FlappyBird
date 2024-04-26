#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
enum class Difficulty {
    Easy,
    Medium,
    Hard
};

struct DifficultySetting {
    float gravity;
    float spaceBetweenPipes;  // 管道间的间距
    float count;
};
class FlappyBird {

  std::shared_ptr<sf::RenderWindow> window;
  float gravity, frame, space,count_flag,gravity_incJ,gravity_incK;
int two_bird;
  int count, score;
  sf::Texture bg, flappy, pipe;
  std::shared_ptr<sf::Sprite> background, bird, pipeBottom, pipeTop,bird2;
  std::vector<sf::Sprite> pipes;
  bool gameover, add;
  bool gameStarted;
  sf::Font font;
  sf::Text txt_score, txt_gameover;
     // std::shared_ptr<sf::RenderWindow> window;
    std::map<Difficulty, DifficultySetting> settings;
std::vector<sf::Text> difficultyOptions;
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
    Difficulty chooseDifficulty();
    void applyDifficultySettings(Difficulty difficulty);
void displayDifficultyMenu();
};
