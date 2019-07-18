#include "Player.h"
#include "Board.h"
#include<iostream>
//Player Implementations

Player::Player(std::string name)
{
	m_name = name;
}
std::string Player::name() const
{
	return m_name;
}
bool Player::isInteractive() const
{
	return false;
}
Player::~Player()
{

}

//HumanPlayer Implementations

HumanPlayer::HumanPlayer(std::string name) : Player(name)
{}
bool HumanPlayer::isInteractive() const
{
	return true;
}
int HumanPlayer::chooseMove(const Board& b, Side s) const
{
	if (b.beansInPlay(s) == 0)
		return -1;
	int choice = 0;
	while (choice <= 0 || choice > b.holes() || b.beans(s, choice) == 0)
	{
		std::cout << "Choose a hole to play! " << std::endl;
		std::cin >> choice;
		if (choice <= 0 || choice > b.holes() || b.beans(s, choice) == 0)
		{
			if (b.beans(s, choice) == 0)
				std::cout << "That hole has no beans! Try again." << std::endl;
			else
				std::cout << "Please enter a number between 1 and " << b.holes() << "." << std::endl;
		}
	}
	return choice;
}

//BadPlayer Implementations

BadPlayer::BadPlayer(std::string name) : Player(name)
{}
int BadPlayer::chooseMove(const Board& b, Side s) const //Returns the hole with the most beans
{
	if (b.beansInPlay(s) == 0)
		return -1;
	int move = 1;
	for (int i = 2; i <= b.holes(); i++)
	{
		if (b.beans(s, move) < b.beans(s, i))
		{
			move = i;
		}
	}
	return move;
}

//SmartPlayer Implementations

SmartPlayer::SmartPlayer(std::string name) : Player(name)
{}
int SmartPlayer::chooseMove(const Board& b, Side s) const
{
	int move = 1;
	int value = currentValue(b, s);
	findMove(b, s, value, move, 7); //Calls helper function that does all the work in order to add recursive capability
	return move;
}
/*void chooseMove(in: player, board; out: bestHole, value):
		if no move for player exists (i.e., game is over),
			bestHole = -1
			value = value of this position (i.e., +big, -big, or 0)
			return
		if the criterion says we should not search below this node
			bestHole = -1
			value = value of this position
			return
		for every hole h the player can choose
			"make" the move h
			chooseMove(opponent, h2, v2)
			"unmake" the move
			if v2 is better for the player than best seen so far,
				bestHole = h
				value = v2
		return*/
void SmartPlayer::findMove(const Board& b, Side s, int &value, int &bestHole, int levelsToCheck) const
{
	int playerBestHole = 1;
	int opponentWorstHole = 1;
	int tempHole = 1;
	int opponentWorstValue = 1000000; //arbitrarily large number to initialize the worst of the opponent's best value, 1 best value for each move
	int opponentBestValue = -1000000; //arbitrarily small number to initialize the best of the opponent's worst value 
	int playerBestValue;   //This will only be used as an output of findmove if we go twice (land in own pot)
	int tempValue;		   //Also only used as an output of findmove for when we are using findmove for our opponent

	if (b.beansInPlay(NORTH) == 0 || b.beansInPlay(SOUTH) == 0) 
	{	//If either side has no beans and s won, value=1000, if s lost value=-1000; regardless, set bestHole=-1
		bestHole = -1;
		if (s == NORTH)
		{
			if ((b.beans(s, 0) + b.beansInPlay(s)) < (b.beans(SOUTH, 0) + b.beansInPlay(SOUTH)))
			{
				value = -1000;
			}
			else
				value = 1000;
		}
		else
		{
			if ((b.beans(s, 0) + b.beansInPlay(s)) < (b.beans(NORTH, 0) + b.beansInPlay(NORTH)))
			{
				value = -1000;
			}
			else
				value = 1000;
		}
		return;
	}
	//If we have checked enough levels set value to current value
	if (levelsToCheck <= 0)
	{
		bestHole = -1;
		value = currentValue(b, s);
		return;
	}
	for (int i = 1; i <= b.holes(); i++) //For each move that works
	{
		if (b.beans(s, i) != 0) //Can't test when hole has 0 beans
		{						//Make the move with a copy board
			int endHole = 0;
			Side endSide;
			Board copy(b);
			copy.sow(s, i, endSide, endHole);
			//If you end in your own pot go again
			while (endHole == 0)//As long as we are landing in the pot, find and make the best move for us
			{
				findMove(copy, s, playerBestValue, playerBestHole, levelsToCheck - 1);
				copy.sow(s, playerBestHole, endSide, endHole); //if findMove gave us -1 then sow won't work
				//If besthole is -1 because we hit the bottom level then we just break to prevent infinite loop,
				//but if besthole is -1 because we won the game then we return this as our best hole
				if (playerBestHole == -1)
				{
					if (playerBestValue == 1000)
					{
						value = 1000;
						bestHole = i;
						return;
					}
					else 
					{
						break;
					}
				}
			}
			//Capturing
			if (s == SOUTH && endSide == SOUTH)
			{
				if ((copy.beans(s, endHole) == 1) && (copy.beans(NORTH, endHole) != 0))
				{//Want to be sure that our endHole is 1 after the sow and that the hole across from it is nonempty
					copy.moveToPot(NORTH, endHole, s);
					copy.moveToPot(SOUTH, endHole, s);
				}
			}
			else if (s == NORTH && endSide == NORTH)
			{
				if ((copy.beans(s, endHole) == 1) && (copy.beans(SOUTH, endHole) != 0))
				{
					copy.moveToPot(SOUTH, endHole, s);
					copy.moveToPot(NORTH, endHole, s);
				}
			}
			//Given the state of the copy board after our current move i, find and record the worst move for our opponent
			//Whether or not we are on an odd or an even level of levelsToCheck
			if (s == NORTH)
				findMove(copy, SOUTH, tempValue, tempHole, levelsToCheck - 1);
			else
				findMove(copy, NORTH, tempValue, tempHole, levelsToCheck - 1);
			if (levelsToCheck % 2 == 1)
			{
				if (tempValue < opponentWorstValue)
				{
					opponentWorstValue = tempValue;
					opponentWorstHole = i;
				}
			}
			else if (levelsToCheck % 2 == 0)
			{
				if (tempValue > opponentBestValue)
				{
					opponentBestValue = tempValue;
					opponentWorstHole = i;
				}
			}
			//Want to end up finding the best possible move we can make out of all the possible moves, given the best possible move
			//that our opponent could make for each move (tempValue), so out of all the moves we can make we find the one with
			//the worst value for our opponent and record the worst hole for our opponent. Then our best hole is the opponents worst
			//hole, and our best value corresponds to our opponents worst hole
		}
	}
	if (levelsToCheck % 2 == 1)
	{
		value = opponentWorstValue;
		bestHole = opponentWorstHole;
	}
	else if (levelsToCheck % 2 == 0)
	{
		value = opponentBestValue;
		bestHole = opponentWorstHole;
	}
	return;
}
int SmartPlayer::currentValue(const Board& b, Side s) const //evaluate the quality of a given board setup
{
	if (s == NORTH)
		return (b.beans(s, 0) - b.beans(SOUTH, 0));
	else
		return(b.beans(s, 0) - b.beans(NORTH, 0));
}