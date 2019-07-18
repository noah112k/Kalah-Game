#include "Game.h"
#include "Board.h" 
#include "Player.h"

Game::Game(const Board& b, Player* south, Player* north)
{
	m_board = new Board(b);
	m_south = south;
	m_north = north;
	turn = SOUTH;
}
bool Game::isGameOver() const
{
	if (m_board->beansInPlay(SOUTH) == 0 || m_board->beansInPlay(NORTH) == 0)
		return true;
	else return false;
}
void Game::display() const
{
	std::cout << "	North" << std::endl << "     ";
	for (int i = 1; i <= m_board->holes(); i++)
	{
		std::cout << m_board->beans(NORTH, i) << " ";
	}
	std::cout << std::endl << m_board->beans(NORTH, 0) << "		    " << m_board->beans(SOUTH, 0) << std::endl << "     ";
	for (int i = 1; i <= m_board->holes(); i++)
	{
		std::cout << m_board->beans(SOUTH, i) << " ";
	}
	std::cout << std::endl << "	South" << std::endl;

}
void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
	if (!(isGameOver()))
	{
		over = false;
		return;
	}
	over = true;
	if ((m_board->beans(NORTH, 0) + m_board->beansInPlay(NORTH)) < (m_board->beans(SOUTH, 0) + m_board->beansInPlay(SOUTH)))
	{
		hasWinner = true;
		winner = SOUTH;
		return;
	}
	else if ((m_board->beans(SOUTH, 0) + m_board->beansInPlay(SOUTH)) < (m_board->beans(NORTH, 0) + m_board->beansInPlay(NORTH)))
	{
		hasWinner = true;
		winner = NORTH;
		return;
	}
	else
	{
		hasWinner = false;
		return;
	}
}
bool Game::move()
{
	int move;
	Side endSide;
	int endHole = 0;
	int numOfTurns = 0;
	if (isGameOver())
		return false;
	while (endHole == 0 && !(isGameOver()))
	{
		if (numOfTurns > 0)
		{
			display();
		}
		if (turn == SOUTH)
			move = m_south->chooseMove(*m_board, turn);
		else
			move = m_north->chooseMove(*m_board, turn);
		m_board->sow(turn, move, endSide, endHole);
		if (turn == SOUTH && endSide == SOUTH)
		{
			if (m_board->beans(turn, endHole) == 1 && m_board->beans(NORTH, endHole) != 0 && (endHole != 0))
			{
				m_board->moveToPot(NORTH, endHole, turn);
				m_board->moveToPot(SOUTH, endHole, turn);
			}
		}
		else if (turn == NORTH && endSide == NORTH)
		{
			if (m_board->beans(turn, endHole) == 1 && m_board->beans(SOUTH, endHole) != 0 && (endHole != 0))
			{
				m_board->moveToPot(SOUTH, endHole, turn);
				m_board->moveToPot(NORTH, endHole, turn);
			}
		}
		numOfTurns++;
	}
	if (isGameOver())
	{
		for (int i = 1; i <= m_board->holes(); i++) //If the game is done move all the beans on each side to respective pot
		{
			m_board->moveToPot(NORTH, i, NORTH);
			m_board->moveToPot(SOUTH, i, SOUTH);
		}
	}
	if (turn == NORTH)
		turn = SOUTH;
	else
		turn = NORTH;
	return true;
}
void Game::play()
{
	bool gameOver = false;
	bool hasWinner = false;
	Side winner;
	while (!(gameOver))
	{
		if (turn == NORTH)
			std::cout << std::endl << "North's Turn!" << std::endl;
		else
			std::cout << std::endl << "South's Turn!" << std::endl;
		display();
		if ((turn == NORTH && !(m_north->isInteractive())) || (turn == SOUTH && !(m_south->isInteractive())))
		{
			std::cout << "Press enter to see the computer's turn!" << std::endl;
			std::cin.ignore();
		}
		std::cout << std::endl;
		move();
		status(gameOver, hasWinner, winner);
	}
	display();
	if (!(hasWinner))
	{
		std::cout << "Tie game!" << std::endl;
	}
	else
	{
		if (winner == SOUTH)
		{
			std::cout << "South wins!" << std::endl;
		}
		else
			std::cout << "North wins!" << std::endl;
	}
}
int Game::beans(Side s, int hole) const
{
	int beans = 0;
	beans = m_board->beans(s, hole);
	return beans;
}