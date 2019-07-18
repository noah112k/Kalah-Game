#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Side.h"

class Board;
class Player
{
public:
	Player(std::string name);
	std::string name() const;
	virtual bool isInteractive() const;
	virtual int chooseMove(const Board& b, Side s) const = 0;
	virtual ~Player();
private:
	std::string m_name;
};
class HumanPlayer : public Player
{
public:
	HumanPlayer(std::string name);
	bool isInteractive() const;
	int chooseMove(const Board& b, Side s) const;
};
class BadPlayer : public Player
{
public:
	BadPlayer(std::string name);
	int chooseMove(const Board& b, Side s) const;
};
class SmartPlayer : public Player
{
public:
	SmartPlayer(std::string name);
	int chooseMove(const Board& b, Side s) const;
private:
	void findMove(const Board& b, Side s, int &value, int& bestHole, int levelsToCheck) const;
	int currentValue(const Board& b, Side s) const; //evaluate the quality of a given board setup
};

#endif 