#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <ctime>

typedef int grid[9][9];

bool solve(grid myGrid)
{
	//Find empty spot
	int x, y, s = 0;
	for (int i = 0; i < 9 && !s; i++)
	{
		for (int j = 0; j < 9 && !s; j++)
		{
			if (myGrid[i][j] == 0)
			{
				x = i;
				y = j;
				s = 1;
			}
		}
	}

	//No empty spots, solution found
	if (!s)
		return true;

	//Empty spot, determine possible entries
	char nums[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	for (int i = 0, j = y; i < 9; i++)
	{
		nums[(int)myGrid[i][j]] = 0;
	}
	for (int j = 0, i = x; j < 9; j++)
	{
		nums[(int)myGrid[i][j]] = 0;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			nums[(int)myGrid[i + ((int)(x / 3)) * 3][j + ((int)(y / 3)) * 3]] = 0;
		}
	}

	//Try all possibilities and recurse for each
	for (int i = 1; i<10; i++)
		if (nums[i] > 0)
		{
			myGrid[x][y] = i;
			if (solve(myGrid))
				return true;
		}

	//When failed, reset position
	myGrid[x][y] = 0;
	return false;
}

bool solve(grid myGrid, int posOrders[81], int tryOrders[81][9])
{
	/*This variation of solve will test the squares in the orders given in posOrders and
	will test the numbers 1-9 in the orders given in tryOrders. For every grid position
	an order of the numbers 1-9 has to be specified.
	----------------
	Find empty spot*/

	int x, y, pos, s = 0;
	for (int i = 0; i < 81 && !s; i++)
	{
		int col = posOrders[i] % 9;
		int row = floor(posOrders[i] / 9);
		if (myGrid[col][row] == 0)
		{
			x = col;
			y = row;
			pos = i;
			s = 1;			
		}
	}

	//No empty spots, solution found
	if (!s)
		return true;

	//Empty spot, determine possible entries
	char nums[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	for (int i = 0, j = y; i < 9; i++)
	{
		nums[myGrid[i][j]] = 0;
	}
	for (int j = 0, i = x; j < 9; j++)
	{
		nums[myGrid[i][j]] = 0;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			nums[myGrid[i + ((x / 3)) * 3][j + ((y / 3)) * 3]] = 0;
		}
	}

	//Try all possibilities and recurse for each
	for (int i = 0; i < 9; i++)
	{
		int guess=tryOrders[pos][i] + 1;
		if (nums[guess] > 0)
		{
			myGrid[x][y] = guess;
			if (solve(myGrid))
				return true;
		}
	}

	//When failed, reset position
	myGrid[x][y] = 0;
	return false;
}

void printGrid(grid myGrid)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			std::cout << myGrid[i][j];
			if (j == 2 || j == 5)
				std::cout << "|";
		}
		std::cout << std::endl;
		if (i == 2 || i == 5)
		{
			for (int k = 0; k < 11; k++)
				std::cout << "-";
			std::cout << std::endl;
		}
	}
}

void makePosOrders(int orders[81])
{
	for (int i = 0; i < 81; i++)
	{
		orders[i] = i;
	}

	time_t seconds;
	time(&seconds);
	srand((unsigned int) seconds);
	int swaps = rand() % 5000 + 5000;

	for (int i = 0; i < swaps; i++)
	{
		int index1 = rand() % 81;
		int index2 = rand() % 80;

		if (index2 == index1)
			index2++;
		std::swap(orders[index1], orders[index2]);
	}
}

void makeTryOrders(int orders[81][9])
{
	for (int i = 0; i < 81; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			orders[i][j] = j;
		}
	}

	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);
	
	for (int i = 0; i < 81; i++)
	{
		int swaps = rand() % 500 + 500;
		for (int j = 0; j < swaps; j++)
		{
			int index1 = rand() % 9;
			int index2 = rand() % 8;

			if (index2 == index1)
				index2++;
			std::swap(orders[i][index1], orders[i][index2]);
		}
	}
}

int main(void)
{
	grid sudoku =
	{
		{ 0, 0, 0,   0, 0, 0,   0, 0, 0 },
		{ 0, 0, 0,   0, 0, 3,   0, 8, 5 },
		{ 0, 0, 1,   0, 2, 0,   0, 0, 0 },

		{ 0, 0, 0,   5, 0, 7,   0, 0, 0 },
		{ 0, 0, 4,   0, 0, 0,   1, 0, 0 },
		{ 0, 9, 0,   0, 0, 0,   0, 0, 0 },

		{ 5, 0, 0,   0, 0, 0,   0, 7, 3 },
		{ 0, 0, 2,   0, 1, 0,   0, 0, 0 },
		{ 0, 0, 0,   0, 4, 0,   0, 0, 9 }
	};

	printGrid(sudoku);

	int posOrders[81];
	makePosOrders(posOrders);

	int tryOrders[81][9];
	makeTryOrders(tryOrders);

	int start = clock();
	solve(sudoku, posOrders, tryOrders);
	int stop = clock();

	std::cout << "Result:" << std::endl;
	printGrid(sudoku);

	std::cout << "time: " << (stop - start) / double(CLOCKS_PER_SEC) * 1000 << std::endl;

	std::cin.ignore();
	return 0;
}
