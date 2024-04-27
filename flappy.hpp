#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

// 难度级别枚举
enum class Difficulty {
    Easy,
    Medium,
    Hard
};

// 难度设置结构体，包含不同难度下的游戏参数
struct DifficultySetting {
    float gravity;// 重力值
    float spaceBetweenPipes;  // 管道间的间距
    float count;// 用于难度设定的额外计数器（管道数量）
};


// FlappyBird游戏的主类
class FlappyBird {

    float gravity, frame, space,count_flag,bird1Gravity,bird2Gravity;
    int two_bird;// 是否启用双人（鸟）模式
    int count, score; // 计数器和得分
    bool gameover, add; // 游戏结束和控制标志
    bool gameStarted;// 游戏开始标志位
    std::shared_ptr<sf::RenderWindow> window;// 游戏窗口
    sf::Texture bg, flappy, pipe;// 游戏纹理：背景、小鸟和管道
    std::shared_ptr<sf::Sprite> background, bird, pipeBottom, pipeTop,bird2;// 游戏精灵
    std::vector<sf::Sprite> pipes;// 管道精灵数组
    sf::Font font;// 字体
    sf::Text txt_score, txt_gameover;// 得分和游戏结束文本
    // std::shared_ptr<sf::RenderWindow> window;// 难度设置映射
    std::map<Difficulty, DifficultySetting> settings;// 难度选择文本数组
    std::vector<sf::Text> difficultyOptions; // 难度选择文本数组
    std::vector<std::string> leaderboardTexts;// 排行榜文本数组


protected:
    void events(); // 处理事件
    void draw(); // 绘制游戏画面
    void game(); // 游戏逻辑处理
    void movePipes(); // 移动管道
    void handleKeyboardInput(); // 处理键盘输入
    void spawnNewPipes(); // 生成新管道
    void updatePipes(); // 更新管道状态
    void checkCollisions(std::size_t i); // 检查碰撞
    void manageScoring(std::size_t i); // 管理得分
    void setAnimeBird(); // 设置动画小鸟
    void moveBird(); // 移动小鸟

public:
    FlappyBird(); // 构造函数
    void run(); // 运行游戏
    Difficulty chooseDifficulty(); // 选择难度
    void applyDifficultySettings(Difficulty difficulty); // 应用难度设置
    void displayDifficultyMenu(); // 显示难度选择菜单
};
