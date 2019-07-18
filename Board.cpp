#include "Board.h"
Board::Board(int nHoles, int nInitialBeansPerHole)
{
	m_south = new int[nHoles + 1];
	m_north = new int[nHoles + 1];
	for (int i = 0; i < nHoles + 1; i++)
	{
		m_south[i] = 0;
		m_north[i] = 0;
	}
	for (int i = 0; i < nInitialBeansPerHole; i++)
	{
		for (int j = 1; j <= nHoles; j++)
		{
			m_south[j]++;
			m_north[j]++;
		}
	}
	m_holes = nHoles;

}
Board::~Board()
{
	delete[] m_north;
	delete[] m_south;
}
Board::Board(const Board& b)
{
	m_holes = b.m_holes;
	m_south = new int[m_holes + 1];
	m_north = new int[m_holes + 1];
	for (int i = 0; i <= m_holes; i++)
	{
		m_south[i] = b.m_south[i];
		m_north[i] = b.m_north[i];
	}
}
int Board::holes() const
{
	return m_holes;
}
int Board::beans(Side s, int hole) const
{
	if (s == SOUTH)
	{
		return m_south[hole];
	}
	else
		return m_north[hole];
}
int Board::beansInPlay(Side s) const
{
	int total = 0;
	if (s == SOUTH)
	{
		for (int i = 1; i <= m_holes; i++)
		{
			total += m_south[i];
		}
	}
	else
	{
		for (int i = 1; i <= m_holes; i++)
		{
			total += m_north[i];
		}
	}
	return total;
}
int Board::totalBeans() const
{
	int total = 0;
	for (int i = 0; i < m_holes+1; i++)
	{
		total += m_south[i];
	}
	for (int i = 0; i < m_holes+1; i++)
	{
		total += m_north[i];
	}
	return total;
}
bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
	int num = 0;
	int count = hole;
	Side currSide = s;
	if (s == SOUTH && (m_south[hole] == 0 || hole <= 0 || hole > m_holes)) //if hole not valid, return false
		return false;
	if (s == NORTH && (m_north[hole] == 0 || hole <= 0 || hole > m_holes))
		return false;
	if (currSide == SOUTH)//Get the num of beans in the hole, then set equal to 0
	{
		num = m_south[hole];
		m_south[hole] = 0;
	}
	if (currSide == NORTH)
	{
		num = m_north[hole];
		m_north[hole] = 0;
	}
	while (num != 0) //While we still have some beans left to distribute
	{
		if (currSide == SOUTH&&count<m_holes) //Distribute beans till we hit the pot
		{
			count++;
			m_south[count]++;
			num--;
		}
		else if(currSide==NORTH&&count>1) //Distribute beans till we hit the pot
		{
			count--;
			m_north[count]++;
			num--;
		}
		if ((currSide == SOUTH && count >= m_holes) || (currSide == NORTH && count <= 1)) //When we hit the pot
		{
			if (num > 0)
			{
				if (currSide == SOUTH && s == SOUTH) //If its our pot add one
				{
					m_south[0]++;
					num--;
				}
				else if (currSide == NORTH && s == NORTH)
				{
					m_north[0]++;
					num--;
				}
				if (num == 0) //If after adding to pot we have no more to distribute, make the pot our last hle
				{
					count = 0;
				}
				else
				{
					swapSides(currSide);//Swap sides and set count equal to where we will start our distribution from on the other side
					if (currSide == NORTH)
						count = m_holes+1;
					else
						count = 0;
				}
			}
		}
	}
	endSide = currSide;
	endHole = count;
	return true;
}
bool Board::moveToPot(Side s, int hole, Side potOwner)
{
	if (hole <= 0 || hole > m_holes)
		return false;
	int beans = 0;
	if (s == SOUTH)
	{
		beans = m_south[hole];
		m_south[hole] = 0;
	}
	else
	{
		beans = m_north[hole];
		m_north[hole] = 0;
	}
	if (potOwner == SOUTH)
		m_south[0] += beans;
	else
		m_north[0] += beans;		
	return true;
}
bool Board::setBeans(Side s, int hole, int beans)
{
	if (hole<0 || hole>m_holes || beans < 0)
		return false;
	if (s == SOUTH)
		m_south[hole] = beans;
	else
		m_north[hole] = beans;
	return true;
}
void Board::swapSides(Side& s)
{
	if (s == NORTH)
		s = SOUTH;
	else
		s = NORTH;
}
