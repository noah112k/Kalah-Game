#ifndef GAME_H
#define GAME_H

#include "Side.h"
#include <iostream>

class Board;
class Player;

class Game
{
public:
	Game(const Board& b, Player* south, Player* north);
	void display() const;
	void status(bool& over, bool& hasWinner, Side& winner) const;
	bool move();
	void play();
	int beans(Side s, int hole) const;
private:
	Board* m_board;
	Player* m_south;
	Player* m_north;
	Side turn;
	bool isGameOver() const;
};

#endif 
