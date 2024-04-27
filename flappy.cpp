#include "flappy.hpp"
#include "iostream"






/************
 *
 * @brief：FlappyBird 类的构造函数，
 * 负责初始化游戏的窗口、加载游戏资源、
 * 设置初始状态以及配置游戏的UI元素
 *
 * @param：gameStarted 游戏启动标志位
 */
//    bg.loadFromFile("./resources/img/background.png");//加载背景
//    flappy.loadFromFile("./resources/img/flappy.png");//加载小鸟
//    pipe.loadFromFile("./resources/img/pipe.png");//加载管道

//Init all kinds of state value and resouce(bird)
FlappyBird::FlappyBird() : gameStarted(false) {
    // 创建游戏窗口，设置大小、标题和窗口样式
    window = std::make_shared<sf::RenderWindow>(
                              sf::VideoMode(1000, 600),//1000x600分辨率窗口
                              "Flappy Bird",//窗口标题
                              sf::Style::Titlebar | sf::Style::Close//设置为只有标题栏和关闭按钮
    );
    // 设置窗口位置在屏幕左上角
    window->setPosition(sf::Vector2i(0, 0));//窗口位置，左上角
    // 设置窗口的最大帧率为60帧/秒
    window->setFramerateLimit( 60 );//最大帧率60


    //增加错误处理 失败则输出错误信息
    // 加载背景图像
    if (!bg.loadFromFile("./resources/img/background.png")) {
        std::cerr << "Error loading background.png" << std::endl;
//        return false; // 抛出异常
    }

    // 加载小鸟图像
    if (!flappy.loadFromFile("./resources/img/flappy.png")) {
        std::cerr << "Error loading flappy.png" << std::endl;
//        return false; // 抛出异常
    }

    // 加载管道图像
    if (!pipe.loadFromFile("./resources/img/pipe.png")) {
        std::cerr << "Error loading pipe.png" << std::endl;
//        return false; // 抛出异常
    }

    two_bird = 0;
    count = {0};//计数器，记录管道生成频率等等周期性事件
    gameover = add = {false};//游戏结束标志位、添加新管道标志位
    score = {0};//得分
    bird1Gravity  = 0;//J键控制的Bird重力
    bird2Gravity = 0;//K键控制的Bird重力

    // 创建并初始化sprite对象，用于游戏中的图像显示
    background = std::make_shared<sf::Sprite>();//创建sprite对象
    bird = std::make_shared<sf::Sprite>();
    bird2 = std::make_shared<sf::Sprite>();
    pipeBottom = std::make_shared<sf::Sprite>();
    pipeTop = std::make_shared<sf::Sprite>();
    // 为sprite应用纹理
    background->setTexture(bg);//给sprite应用纹理
    bird->setTexture(flappy);
    bird2->setTexture(flappy);
    pipeBottom->setTexture(pipe);
    pipeTop->setTexture(pipe);

    // 设置小鸟和管道的初始位置和大小
    bird2->setPosition(
        300.f - flappy.getSize().x / 2.f,//设置小鸟的初始位置在窗口中间
        300.f -  flappy.getSize().y / 2.f
    );

    bird->setPosition(
        500.f - flappy.getSize().x / 2.f,//设置小鸟的初始位置在窗口中间
        300.f -  flappy.getSize().y / 2.f
    );

    bird->setScale(2.f, 2.f);//小鸟大小
    bird->setTextureRect(sf::IntRect(0, 0, 34, 24));//纹理矩形，用于碰撞判断

    bird2->setScale(2.f, 2.f);//小鸟大小
    bird2->setTextureRect(sf::IntRect(0, 0, 34, 24));//纹理矩形，用于碰撞判断


    pipeBottom->setScale(1.5f, 1.5f);//上下管道，负，反向缩放
    pipeTop->setScale(1.5f, -1.5f);


    // 加载字体文件
    font.loadFromFile("./resources/font/flappybird.ttf");//字体

    // 设置游戏结束和得分的显示属性
    txt_gameover.setFont(font);
    txt_gameover.setString("Press SPACE to restart");
    txt_gameover.setPosition(200, 50);
    txt_gameover.setCharacterSize(50);
    txt_gameover.setOutlineThickness(3);

    txt_score.setFont(font);
    txt_score.setString(std::to_string(score));
    txt_score.setPosition(10.f, 10.f);
    txt_score.setCharacterSize(50);
    txt_score.setOutlineThickness(3);
}



// displayDifficultyMenu 方法用于显示并处理游戏难度选择菜单
void FlappyBird::displayDifficultyMenu() {

    //错误处理：字体和图像加载时进行错误检查
    // 确保字体已加载  flappybird.ttf
    if (!font.loadFromFile("./resources/font/flappybird.ttf")) {
        std::cerr << "Failed to load font\n";
        return;
    }

    // 创建表示不同难度级别的文本对象
    sf::Text easy("Easy", font, 50);
    sf::Text medium("Medium", font, 50);
    sf::Text hard("Hard-2P", font, 50);

    // 设置文本的位置
    easy.setPosition(700, 150);
    medium.setPosition(700, 250);
    hard.setPosition(700, 350);

    // 设置文本的颜色（可选）
    easy.setFillColor(sf::Color::Green);
    medium.setFillColor(sf::Color::Yellow);
    hard.setFillColor(sf::Color::Red);

    //two_bird = 0;
    gravity =  frame = {0.f};//初始化重力、帧率为0
    space = {220.f};//管道之间的空隙
    count_flag ={ 0};
    bool difficultyChosen = false;// 用于跟踪是否已选择难度
    // 游戏窗口打开时循环，直到选择了难度
    while (!difficultyChosen && window->isOpen()) {
        sf::Event event;
        // 处理所有窗口事件
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            // 检测鼠标点击事件，用于选择难度
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window->mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                    // 根据鼠标位置判断点击的难度选项并应用设置
                    if (easy.getGlobalBounds().contains(mousePos)) {
                        applyDifficultySettings(Difficulty::Easy);
                        difficultyChosen = true;
                    } else if (medium.getGlobalBounds().contains(mousePos)) {
                        applyDifficultySettings(Difficulty::Medium);
                        difficultyChosen = true;
                    } else if (hard.getGlobalBounds().contains(mousePos)) {
                        applyDifficultySettings(Difficulty::Hard);
                        two_bird = 1;// 特定难度可能触发双（人）鸟模式
                        difficultyChosen = true;
                    }
                }
            }
        }

        // 重新绘制背景和主界面（如果需要）
        // drawBackground();
        // drawMainMenu();

        // 绘制难度选择文本
        window->draw(easy);
        window->draw(medium);
        window->draw(hard);
        window->display();
    }
}
//Difficulty FlappyBird::chooseDifficulty() {
//    std::cout << "Choose difficulty:\n";
//    std::cout << "1. Easy\n";
//    std::cout << "2. Medium\n";
//    std::cout << "3. Hard\n";
//    int choice;
//    std::cin >> choice;
//    switch (choice) {
//        case 1: return Difficulty::Easy;
//        case 2: return Difficulty::Medium;
//        case 3: return Difficulty::Hard;
//        default: return Difficulty::Easy;  // 默认为简单难度
//    }
//}
//Difficulty FlappyBird::chooseDifficulty() {
//    std::string input;
//    int choice = 0;
//
//    std::cout << "Choose difficulty:\n";
//    std::cout << "1. Easy\n";
//    std::cout << "2. Medium\n";
//    std::cout << "3. Hard\n";
//
//    while (true) {
//        std::getline(std::cin, input);
//        std::stringstream ss(input);
//        if (ss >> choice && choice >= 1 && choice <= 3) {
//            break;
//        } else {
//            std::cout << "Invalid input. Please enter a number (1-3):\n";
//        }
//    }
//
//    switch (choice) {
//        case 1: return Difficulty::Easy;
//        case 2: return Difficulty::Medium;
//        case 3: return Difficulty::Hard;
//        default: return Difficulty::Easy;  // 默认为简单难度，理论上不会到达这里
//    }
//}


void FlappyBird::applyDifficultySettings(Difficulty difficulty) {
    settings = {
            {Difficulty::Easy, {0.1f, 300.f, 150.f}},  // 容易模式：较低的重力，较大的管道间隔和较高的计数器
            {Difficulty::Medium, {0.4f, 250.f, 100.f}},  // 中等模式：中等的重力，中等的管道间隔和中等的计数器
            {Difficulty::Hard, {0.1f, 300.f, 150.f}}  // 困难模式：高重力，较小的管道间隔和较低的计数器
    };

    // 从映射表中获取当前选择难度的设置
    DifficultySetting setting = settings[difficulty];
    // 应用难度设置：设置重力、管道间隔和计数器
    gravity = setting.gravity;  // 设置重力
    space = setting.spaceBetweenPipes;  // 设置管道间的空间
    count_flag = setting.count;  // 设置相关的计数器，可能用于调整游戏节奏或其他逻辑
}



// saveScore 函数用于将玩家的分数保存到文件中
void saveScore(int score) {
    // 打开文件，以追加模式写入，确保不会覆盖现有内容
    std::ofstream scoreFile("scores.txt", std::ios::app);

    // 检查文件是否成功打开
    if (scoreFile.is_open()) {
        scoreFile << score << std::endl;  // 写入分数并换行，便于读取时分割
        scoreFile.close();  // 写入完成后关闭文件
    }
}


// loadScores 函数用于从文件中加载所有之前保存的分数
std::vector<int> loadScores() {
    std::vector<int> scores;  // 用于存储加载的分数
    std::ifstream scoreFile("scores.txt");  // 打开分数文件以读取
    std::string line;  // 用于存储从文件中读取的每一行

    // 循环读取文件中的每一行，直到文件结束
    while (getline(scoreFile, line)) {
        scores.push_back(std::stoi(line));  // 将读取的行转换为整数，并添加到分数列表中
    }
    scoreFile.close();  // 读取完成后关闭文件

    return scores;  // 返回包含所有分数的向量
}



//轮循 事件回调判断状态
//这里只判断游戏是否结束
//其他功能后续添加
void FlappyBird::events(){
    auto e = std::make_shared<sf::Event>();
    while( window->pollEvent( *e ) ){
        if( e->type == sf::Event::Closed){
          window->close();
        } else if (e->type == sf::Event::KeyPressed) {
          if (!gameStarted) {
            gameStarted = true;
          }
        }
    }

    if(gameover && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        saveScore(score);  // 保存当前分数
        std::vector<int> scores = loadScores();  // 加载所有保存的分数
        std::sort(scores.begin(), scores.end(), std::greater<int>());  // 将分数从高到低排序

        // 清空并更新排行榜文本
        leaderboardTexts.clear();
        for (int i = 0; i < scores.size() && i < 10; ++i) {
            leaderboardTexts.push_back("Top " + std::to_string(i + 1) + ": " + std::to_string(scores[i]));
        }

        // 重置游戏状态
        score = 0;
        txt_score.setString(std::to_string(score));  // 更新分数显示
        pipes.clear();  // 清除所有管道
        // 重置小鸟的位置
        bird->setPosition(
                500.f - flappy.getSize().x / 2.f,
                300.f - flappy.getSize().y / 2.f
        );
        bird2->setPosition(
                300.f - flappy.getSize().x / 2.f,
                300.f - flappy.getSize().y / 2.f
        );
        gameover = false;  // 清除游戏结束标志
        gameStarted = false;  // 游戏未开始状态重置
    }
}

// draw 方法用于渲染游戏中的所有可视元素
void FlappyBird::draw(){
    window->clear(sf::Color::Black);  // 清空窗口内容，以黑色填充背景

    window->draw(*background);  // 绘制背景图

    // 遍历并绘制所有管道
    for(auto &p : pipes){
        window->draw(p);
    }

    // 根据是否启用双鸟模式绘制小鸟
    if(two_bird == 0){
        window->draw(*bird);  // 如果未启用双鸟模式，仅绘制一个小鸟
    } else if(two_bird == 1){
        window->draw(*bird2);  // 如果启用了双鸟模式，绘制第二只小鸟
        window->draw(*bird);   // 继续绘制第一只小鸟
    }

    // 如果游戏结束，则绘制游戏结束的文本
    if(gameover){
        window->draw(txt_gameover);

        // 显示排行榜
        float yPos = 150.f;  // 排行榜开始的y坐标位置
        for(const auto& text : leaderboardTexts){
            sf::Text rankText;
            rankText.setFont(font);  // 设置字体
            rankText.setString(text);  // 设置文本内容
            rankText.setCharacterSize(36);  // 设置文本大小
            rankText.setFillColor(sf::Color::White);  // 设置文本颜色为白色
            rankText.setPosition(450, yPos);  // 设置文本位置
            window->draw(rankText);  // 绘制文本
            yPos += 40.f;  // 每绘制一行文本后，更新y坐标位置，为下一行文本留出空间
        }
    }

    window->draw(txt_score);  // 绘制当前得分
    window->display();  // 显示所有绘制的内容
}



// run 方法封装了游戏的主循环，控制游戏的执行流程
void FlappyBird::run(){
    draw();  // 初始绘制游戏画面
    displayDifficultyMenu();  // 显示难度选择菜单

    // 当游戏窗口开启时，持续执行游戏循环
    while(window->isOpen()){
        events();  // 处理所有游戏事件，如键盘输入和窗口关闭事件
        game();    // 处理游戏逻辑，包括小鸟的移动、碰撞检测和分数更新
        draw();    // 绘制当前游戏状态到窗口上

        ++count;  // 增加循环计数器，可用于调整游戏难度等周期性事件

        // 重置计数器，预留采样节点
        if(count == 300){
            count = 0;  // 每300帧重置计数器，可在此处加入逻辑如难度增减
        }
    }
}



//管道的事件：移动、增添
void FlappyBird::movePipes(){
	if (gameStarted) {
    // 管道移动和生成的代码保持不变
        printf("%d\r",gravity);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
            bird1Gravity  = -8.f;
            bird->setRotation(-frame);//根据帧数，旋转小鸟
        } else {
            // 如果 J 没有被按下，则使用默认的重力
           // gravity += 0.5f;
            bird->setRotation(frame);//根据帧数，旋转小鸟
           // bird->move(0, gravity);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
            bird2Gravity = -8.f;
            bird->setRotation(-frame);//根据帧数，旋转小鸟
            // bird2Gravity = -1.f; // 控制第二只小鸟向上移动
            //bird2->move(0, gravity);
        } else {
            // 如果 K 没有被按下，则使用默认的重力
            // gravity += 0.5.f;
            //bird2->move(0, gravity);bird->setRotation(-frame - 10.f);//根据帧数，旋转小
            bird->setRotation(frame);//根据帧数，旋转小鸟
        }
        if( count %(int) count_flag == 0 ){//主循环采样一半，就会生成新管道，可以控制管道的生成速度
            int pos = std::rand() % 275 + 175;//纵向位置随机，

            pipeBottom->setPosition(1000, pos + space);//端点固定在1000（窗口）
            pipeTop->setPosition(1000, pos);//确保每次都出现在屏幕外，然后移动进来

            pipes.push_back(*pipeBottom);
            pipes.push_back(*pipeTop);//移动
        }
        for (std::size_t i {}; i < pipes.size(); ++i) {//遍历管道。判断碰撞
            if(two_bird == 1 ){
                if( pipes[i].getGlobalBounds().intersects(bird->getGlobalBounds())){
                    bird->move(15.f, 0);//发生碰撞就移动（向右），15

                      if( pipes[i].getScale().y < 0 ){//根据管道的方向调整小鸟的头
                        bird->move(0, -15.f);
                      }else{
                        bird->move(0, 15.f);
                      }
                      gameover = true;//结束标志位
                }
                if( pipes[i].getGlobalBounds().intersects(bird2->getGlobalBounds())){
                  bird2->move(15.f, 0);//发生碰撞就移动（向右），15

                  if( pipes[i].getScale().y < 0 ){//根据管道的方向调整小鸟的头
                    bird2->move(0, -15.f);
                  }else{
                    bird2->move(0, 15.f);
                  }
	                gameover = true;//结束标志位
                }

            }
            if(two_bird == 0){
               if( pipes[i].getGlobalBounds().intersects(bird->getGlobalBounds())){
                  bird->move(15.f, 0);//发生碰撞就移动（向右），15
                  if( pipes[i].getScale().y < 0 ){//根据管道的方向调整小鸟的头
                    bird->move(0, -15.f);
                  }else{
                    bird->move(0, 15.f);
                  }
                  gameover = true;//结束标志位
                }
            }

           if( pipes[i].getPosition().x < -100 ){
             pipes.erase(pipes.begin() + i );
           } //如果管道移动到了左侧外面，删除

           pipes[i].move(-4.f, 0);//每次迭代都移动4，模拟管道向小鸟移动的效果

           if(pipes[i].getPosition().x == 448 && !add ){//记录分数、根据管道位置判断的
                //add防止误判。
                std::cout << pipes[i].getPosition().x << '\n';
                txt_score.setString(std::to_string(++score));
                add = true;
           }else{
                add = false;
           }

        }
    }
}
//void FlappyBird::movePipes(){
//    if (!gameStarted) return;
//
//    handleKeyboardInput();
//    spawnNewPipes();
//    updatePipes();
//}
//
//void FlappyBird::handleKeyboardInput() {
//    float rotationAdjustment = (sf::Keyboard::isKeyPressed(sf::Keyboard::J) || sf::Keyboard::isKeyPressed(sf::Keyboard::K)) ? -frame : frame;
//    bird->setRotation(rotationAdjustment);
//
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
//        bird1Gravity = -8.f;
//    }
//    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {
//        bird2Gravity = -8.f;
//    }
//}
//
//void FlappyBird::spawnNewPipes() {
//    if (count % static_cast<int>(count_flag) == 0) {
//        int pos = std::rand() % 275 + 175;
//        pipeBottom->setPosition(1000, pos + space);
//        pipeTop->setPosition(1000, pos);
//        pipes.push_back(*pipeBottom);
//        pipes.push_back(*pipeTop);
//    }
//}
//
//void FlappyBird::updatePipes() {
//    for (std::size_t i = 0; i < pipes.size(); ++i) {
//        pipes[i].move(-4.f, 0);
//        if (pipes[i].getPosition().x < -100) {
//            pipes.erase(pipes.begin() + i--);
//            continue;
//        }
//        checkCollisions(i);
//        manageScoring(i);
//    }
//}
//
//void FlappyBird::checkCollisions(std::size_t i) {
//    sf::Sprite* birds[] = {bird, bird2};
//    for (int b = 0; b < (two_bird ? 2 : 1); b++) {
//        if (pipes[i].getGlobalBounds().intersects(birds[b]->getGlobalBounds())) {
//            birds[b]->move(15.f, 0);
//            float yMove = (pipes[i].getScale().y < 0) ? -15.f : 15.f;
//            birds[b]->move(0, yMove);
//            gameover = true;
//        }
//    }
//}
//
//void FlappyBird::manageScoring(std::size_t i) {
//    if (pipes[i].getPosition().x == 448 && !add) {
//        std::cout << pipes[i].getPosition().x << '\n';
//        txt_score.setString(std::to_string(++score));
//        add = true;
//    } else {
//        add = false;
//    }
//}
//
//
//
//
//
//void FlappyBird::game(){
//  if(!gameover){//游戏核心逻辑了
//    setAnimeBird();//模拟小鸟扑动，改变纹理实现的
//    moveBird();//小鸟上下，根据g改变小鸟位置，模拟重力
//    movePipes();//管道的逻辑，移动的逻辑
//    //printf("%d\r\n",two_bird);
//  }
//}
//
//void FlappyBird::setAnimeBird(){//小鸟动画
//  frame += 0.15f;//frame控制当前帧
//  if( frame > 3 ){
//    frame -= 3;
//  }
//    if(two_bird == 0)
//        bird->setTextureRect(sf::IntRect( 34 * (int)frame, 0, 34, 24 ));//决定了显示纹理的那一部分
//    if(two_bird == 1){
//        bird->setTextureRect(sf::IntRect( 34 * (int)frame, 0, 34, 24 ));
//        bird2->setTextureRect(sf::IntRect( 34 * (int)frame, 0, 34, 24 ));
//    }
//}



// moveBird 方法用于更新小鸟的位置，并检查是否超出窗口边界
void FlappyBird::moveBird() {
    // 只有当游戏已开始时才处理移动逻辑
    if (gameStarted) {
        // 如果启用双鸟模式
        if (two_bird == 1) {
            // 分别移动两只小鸟，并逐步增加下落速度
            bird->move(0, bird1Gravity);
            bird2->move(0, bird2Gravity);
            bird1Gravity += 0.5f;  // 增加第一只小鸟的下落速度
            bird2Gravity += 0.5f;  // 增加第二只小鸟的下落速度

            // 检查第一只小鸟是否移出窗口边界
            if (bird->getPosition().y > window->getSize().y || bird->getPosition().y < 0) {
                gameover = true;  // 如果超出边界，设置游戏结束标志
            }
            // 检查第二只小鸟是否移出窗口边界
            if (bird2->getPosition().y > window->getSize().y || bird2->getPosition().y < 0) {
                gameover = true;  // 如果超出边界，设置游戏结束标志
            }
        }
        // 如果未启用双鸟模式
        if (two_bird == 0) {
            // 移动单只小鸟，并逐步增加下落速度
            bird->move(0, bird1Gravity);
            bird1Gravity += 0.5f;  // 增加小鸟的下落速度

            // 检查小鸟是否移出窗口边界
            if (bird->getPosition().y > window->getSize().y || bird->getPosition().y < 0) {
                gameover = true;  // 如果超出边界，设置游戏结束标志
            }
        }
    }
}


