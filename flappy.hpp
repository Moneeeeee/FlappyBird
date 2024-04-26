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

    float gravity, frame, space,count_flag,bird1Gravity,bird2Gravity;
    int two_bird;
    int count, score;
    bool gameover, add;
    bool gameStarted;
    std::shared_ptr<sf::RenderWindow> window;
    sf::Texture bg, flappy, pipe;
    std::shared_ptr<sf::Sprite> background, bird, pipeBottom, pipeTop,bird2;
    std::vector<sf::Sprite> pipes;
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
        void handleBirdMovement();
        void spawnPipes();
        void updatePipes();
        void handleCollisions(std::size_t pipeIndex);
        void checkCollision(sf::Sprite* bird, sf::Sprite& pipe);
};
