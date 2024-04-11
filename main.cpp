#include "flappy.hpp"

int main(){//Enter function

	//Init the randomnumber generator using the current time
	//Make sure ecery time the game star with a different seed
	std::srand( std::time(0));
	//make_shared manage its life T,automatically guess the type
	auto flappy = std::make_shared<FlappyBird>();
	//while
	flappy->run();
	return 0;
}

