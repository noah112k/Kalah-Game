#include "Board.h"
#include "Player.h"
#include "Game.h"
#include <iostream>
#include <cassert>
int main()
{
	
	//Ensure that board and sow work correctly
	Board b(6, 3);
	assert(b.holes() == 6);
	assert(b.beans(NORTH, 1) == 3);
	assert(b.beans(NORTH, 0) == 0);
	Side end = NORTH;
	int endHole = 0;
	assert(b.sow(NORTH, 1, end, endHole));
	assert(b.beans(NORTH, 1) == 0);
	assert(b.beans(NORTH, 0) == 1);
	assert(b.beans(SOUTH, 1) == 4);
	
	//Test functionality of board, game, and badplayer
	Board c(6, 3);
	BadPlayer bad1("Bad1");
	BadPlayer bad2("Bad2");
	Game test(c, &bad1, &bad2);
	test.play();

	//Test functionality of an interactive player playing with a noninteractive one
	HumanPlayer user("me");
	Game test2(c, &user, &bad2);

	//Test functionality of two smartplayers playing against one another
	Board d(6, 3);
	SmartPlayer p1("South");
	SmartPlayer p2("North");
	Game g(c, &p1, &p2);
	g.play();
	std::cout << "Good so far! " << std::endl;
}