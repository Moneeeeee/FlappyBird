#include "flappy.hpp"
#include "iostream"

//Init all kinds of state value and resouce(bird)
FlappyBird::FlappyBird() : gameStarted(false) {
    window = std::make_shared<sf::RenderWindow>(
                              sf::VideoMode(1000, 600),//1000x600分辨率窗口
                              "Flappy Bird",//窗口标题
                              sf::Style::Titlebar | sf::Style::Close//设置为只有标题栏和关闭按钮
    );

    window->setPosition(sf::Vector2i(0, 0));//窗口位置，左上角
    window->setFramerateLimit( 60 );//最大帧率60

//    bg.loadFromFile("./resources/img/background.png");//加载背景
//    flappy.loadFromFile("./resources/img/flappy.png");//加载小鸟
//    pipe.loadFromFile("./resources/img/pipe.png");//加载管道
    //增加错误处理
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

    background = std::make_shared<sf::Sprite>();//创建sprite对象
    bird = std::make_shared<sf::Sprite>();
    bird2 = std::make_shared<sf::Sprite>();
    pipeBottom = std::make_shared<sf::Sprite>();
    pipeTop = std::make_shared<sf::Sprite>();

    background->setTexture(bg);//给sprite应用纹理
    bird->setTexture(flappy);
    bird2->setTexture(flappy);
    pipeBottom->setTexture(pipe);
    pipeTop->setTexture(pipe);


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



    font.loadFromFile("./resources/font/flappybird.ttf");//字体

    //游戏结束显示的文字 啥的
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




void FlappyBird::displayDifficultyMenu() {

    //错误处理：字体和图像加载时进行错误检查
    // 确保字体已加载  flappybird.ttf
    if (!font.loadFromFile("./resources/font/flappybird.ttf")) {
        std::cerr << "Failed to load font\n";
        return;
    }

    sf::Text easy("Easy", font, 50);
    sf::Text medium("Medium", font, 50);
    sf::Text hard("Hard", font, 50);

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
    bool difficultyChosen = false;

    while (!difficultyChosen && window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window->mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                    if (easy.getGlobalBounds().contains(mousePos)) {
                        applyDifficultySettings(Difficulty::Easy);
                        difficultyChosen = true;
                    } else if (medium.getGlobalBounds().contains(mousePos)) {
                        applyDifficultySettings(Difficulty::Medium);
                        difficultyChosen = true;
                    } else if (hard.getGlobalBounds().contains(mousePos)) {
                        applyDifficultySettings(Difficulty::Hard);
                        two_bird = 1;
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
            {Difficulty::Easy, {0.1f, 300.f, 150.f}},
            {Difficulty::Medium, {0.4f, 250.f,100.f}},
            {Difficulty::Hard, {0.1f, 300.f,150.f}}
    };

    DifficultySetting setting = settings[difficulty];
    gravity = setting.gravity;
    space = setting.spaceBetweenPipes;
    count_flag = setting.count;
}



void saveScore(int score) {
    std::ofstream scoreFile("scores.txt", std::ios::app);
    if (scoreFile.is_open()) {
        scoreFile << score << std::endl;
        scoreFile.close();
    }
}

std::vector<int> loadScores() {
    std::vector<int> scores;
    std::ifstream scoreFile("scores.txt");
    std::string line;
    while (getline(scoreFile, line)) {
        scores.push_back(std::stoi(line));
    }
    scoreFile.close();
    return scores;
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

    if(gameover && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        saveScore(score);  // 保存当前分数
        std::vector<int> scores = loadScores();  // 加载所有分数
        std::sort(scores.begin(), scores.end(), std::greater<int>());  // 排序分数

        leaderboardTexts.clear();
        for (int i = 0; i < scores.size() && i < 10; ++i) {
          leaderboardTexts.push_back("Top " + std::to_string(i + 1) + ": " + std::to_string(scores[i]));
        }

        score = 0;
        txt_score.setString(std::to_string(score));
        pipes.clear();
        bird->setPosition(
          500.f - flappy.getSize().x / 2.f,
          300.f - flappy.getSize().y / 2.f
        );
        bird2->setPosition(
          300.f - flappy.getSize().x / 2.f,
          300.f - flappy.getSize().y / 2.f
        );
        gameover = false;
        gameStarted = false;
    }
}

void FlappyBird::draw(){
    window->clear(sf::Color::Black);  // 清屏
    window->draw(*background);  // 填充背景

    for(auto &p : pipes){
        window->draw(p);  // 绘制管道
    }
    if(two_bird==0){
        window->draw(*bird);  // 绘制小鸟
    }else if(two_bird == 1){
        window->draw(*bird2);
        window->draw(*bird);
    }
    if(gameover){
        window->draw(txt_gameover);  // 绘制游戏结束文本
        // 显示排行榜
        float yPos = 150.f;  // 排行榜开始的y位置
        for(const auto& text : leaderboardTexts){
          sf::Text rankText;
          rankText.setFont(font);
          rankText.setString(text);
          rankText.setCharacterSize(36);
          rankText.setFillColor(sf::Color::White);
          rankText.setPosition(450, yPos);
          window->draw(rankText);
          yPos += 40.f;  // 更新位置，以便下一行文本
        }
    }
    window->draw(txt_score);  // 绘制得分
    window->display();
}


//封装了主循环，轮询响应事件
void FlappyBird::run(){
    draw();//绘制总函数
    displayDifficultyMenu();

      while( window->isOpen() ){
        events();//时间总处理
        game();//小鸟移动、检查碰撞、更新分数等内容
        draw();//绘制总函数

        ++count;
    //预留采样节点、后续增减难度可用
        if( count == 300 ){
          count = 0;
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


void FlappyBird::handleCollisions(std::size_t pipeIndex) {
    auto& pipe = pipes[pipeIndex];
    if (two_bird) {
        checkCollision(bird, pipe);
        checkCollision(bird2, pipe);
    } else {
        checkCollision(bird, pipe);
    }
}

void FlappyBird::checkCollision(sf::Sprite* bird, sf::Sprite& pipe) {
    if (pipe.getGlobalBounds().intersects(bird->getGlobalBounds())) {
        bird->move(15.f, 0);
        float yMove = (pipe.getScale().y < 0) ? -15.f : 15.f;
        bird->move(0, yMove);
        gameover = true;
    }
}




void FlappyBird::game(){
  if(!gameover){//游戏核心逻辑了
    setAnimeBird();//模拟小鸟扑动，改变纹理实现的
    moveBird();//小鸟上下，根据g改变小鸟位置，模拟重力
    movePipes();//管道的逻辑，移动的逻辑
    //printf("%d\r\n",two_bird);
  }
}

void FlappyBird::setAnimeBird(){//小鸟动画
  frame += 0.15f;//frame控制当前帧
  if( frame > 3 ){
    frame -= 3;
  }
    if(two_bird == 0)
        bird->setTextureRect(sf::IntRect( 34 * (int)frame, 0, 34, 24 ));//决定了显示纹理的那一部分
    if(two_bird == 1){
        bird->setTextureRect(sf::IntRect( 34 * (int)frame, 0, 34, 24 ));
        bird2->setTextureRect(sf::IntRect( 34 * (int)frame, 0, 34, 24 ));
    }
}


// 修改moveBird函数
void FlappyBird::moveBird(){
    if (gameStarted) {
        if(two_bird == 1){
            bird->move(0, bird1Gravity );
            bird2->move(0, bird2Gravity);
            bird1Gravity  += 0.5f;
            bird2Gravity += 0.5f;
            // Check if bird is out of window bounds
            if(bird->getPosition().y > window->getSize().y || bird->getPosition().y < 0) {
              gameover = true;
            }
            if(bird2->getPosition().y > window->getSize().y || bird2->getPosition().y < 0) {
              gameover = true;
          }
        }
        if(two_bird == 0){
            bird->move(0, bird1Gravity );
            bird1Gravity  += 0.5f;
        if(bird->getPosition().y > window->getSize().y || bird->getPosition().y < 0) {
              gameover = true;
            }
        }


    }
}

