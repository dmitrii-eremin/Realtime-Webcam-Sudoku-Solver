#include "stdafx.h"
#include "SudSolver.h"


SudSolver::SudSolver(void)
{	ZeroMemory(cell, sizeof(SudSolverCell)*9*9);
	m_BruteForceStep = 0;

	// set all candidates to TRUE initially
	for (int y=0; y<9; y++)
		for (int x=0; x<9; x++)
			for (int candidate=1; candidate<10; candidate++)
				cell[y][x].isCandidate[candidate] = TRUE;
}

SudSolver::~SudSolver(void)
{}

CellOrder SudSolver::m_cellOrder[81] = {0};

//////////////////////////////////////////////////////////////////////////
// Function:	AreAllCellsValid()
// Purpose:		Calculates whether the sudoku values are valid. This means that every number must be unique for row, column and 3x3 block.
// Parameters:
// Returns:		TRUE if values are valid, otherwise FALSE. 
/////////////////////////////////////////////////////////////////////////
BOOL SudSolver::AreAllCellsValid(void)
{
	int cellCount = 0;
	for (int y=0; y<9; y++)
		for (int x=0; x<9; x++)
			if (cell[y][x].value != 0)
				if (!IsValidCell(y, x))
					return FALSE;
				else
					cellCount++;
	if (cellCount > 11)	// skip incomplete sudoku grid. There must be at least 12 digits in it to be a valid grid.
		return TRUE;
	else
		return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Function:	Step1()
// Purpose:		Tries to solve the sudoku puzzle with "naked single candidate" method.
//				It checks to see if any of the cells have ONLY ONE possible value - only one possible candidate.
//				Step 1. Updates cell[y][x].isCandidate array with all the possible values.
//				Step 2. Searches for single candidates.
//				If any are found the value is entered for this cell and Step 1 is repeated until all remaining unsolved cells contain multiple candidates
// Parameters:
// Returns:		TRUE if entire puzzle solved, otherwise FALSE. 
/////////////////////////////////////////////////////////////////////////
BOOL SudSolver::Step1(void)
{
	int x, y, candidate, value, count;
	BOOL bFoundAtLeastOne;		// TRUE if found at least one single possible solution
	BOOL bSolved;				// TRUE if the entire sudoku is solved

	do {
		bFoundAtLeastOne = FALSE;
		bSolved = TRUE;
		// for each cell update the candidates
		for (y=0; y<9; y++)
		{	for (x=0; x<9; x++)
			{	if (cell[y][x].value == 0)								// only empty cells, please
				{	count = 0;
					for (candidate=1; candidate<10; candidate++)		// for each cell try with all possible candidates
					{	if (cell[y][x].isCandidate[candidate] == TRUE)	// only candidates, please
						{	cell[y][x].value = candidate;
							if (IsValidCell(y, x) == TRUE)
							{	count++;
								value = candidate;
							}
							else
								cell[y][x].isCandidate[candidate] = FALSE;
						}
					}
					if (count == 1)
					{	cell[y][x].value = value;	// set the only possible cell value
						bFoundAtLeastOne = TRUE;	// and signal the do-while loop to go deeper with the next loop
					}
					else
					{	cell[y][x].value = 0;		// reset the cell
						bSolved = FALSE;			// sudoku is not solved
					}
				}
			}
		}
	} while (bFoundAtLeastOne == TRUE);

	return bSolved;
}

//////////////////////////////////////////////////////////////////////////
// Function:	Step2()
// Purpose:		Tries to solve the sudoku puzzle with "hidden single candidate" method.
//				Searches for only one candidate value in a 3x3block or row or column.
//				This single candidate is a valid value.
//				The array cell[y][x].isCandidate[] must be filled with valid candidates before calling this function.
// Parameters:
// Returns:		TRUE if at least one value found, otherwise FALSE.
/////////////////////////////////////////////////////////////////////////
BOOL SudSolver::Step2(void)
{
	int x, y, candidate, index, count;
	static const int xx[81] = {0,1,2,0,1,2,0,1,2, 3,4,5,3,4,5,3,4,5, 6,7,8,6,7,8,6,7,8,   0,1,2,0,1,2,0,1,2, 3,4,5,3,4,5,3,4,5, 6,7,8,6,7,8,6,7,8,   0,1,2,0,1,2,0,1,2, 3,4,5,3,4,5,3,4,5, 6,7,8,6,7,8,6,7,8};
	static const int yy[81] = {0,0,0,1,1,1,2,2,2, 0,0,0,1,1,1,2,2,2, 0,0,0,1,1,1,2,2,2,   3,3,3,4,4,4,5,5,5, 3,3,3,4,4,4,5,5,5, 3,3,3,4,4,4,5,5,5,   6,6,6,7,7,7,8,8,8, 6,6,6,7,7,7,8,8,8, 6,6,6,7,7,7,8,8,8};

	// for each value candidate
	for (candidate=1; candidate<10; candidate++)
	{	// for each row
		for (y=0; y<9; y++)
		{	count = 0;
			for (x=0; x<9; x++)
			{	if (cell[y][x].value == 0)				// only unsolved cells, please
				{	if (cell[y][x].isCandidate[candidate] == TRUE)
					{	count++;
						index = x;
			}	}	}
			if (count == 1)
			{	cell[y][index].value = candidate;		// bingo
				return TRUE;
			}
		}
		// for each column
		for (x=0; x<9; x++)
		{	count = 0;
			for (y=0; y<9; y++)
			{	if (cell[y][x].value == 0)				// only unsolved cells, please
				{	if (cell[y][x].isCandidate[candidate] == TRUE)
					{	count++;
						index = y;
			}	}	}
			if (count == 1)
			{	cell[index][x].value = candidate;		// bingo
				return TRUE;
			}
		}
		// for each 3x3 block
		int i = 0;
		for (y=0; y<9; y++)
		{	count = 0;
			for (x=0; x<9; x++)
			{	if (cell[yy[i]][xx[i]].value == 0)			// only unsolved cells, please
				{	if (cell[yy[i]][xx[i]].isCandidate[candidate] == TRUE)
					{	count++;
						index = i;
				}	}
				i++;
			}
			if (count == 1)
			{	cell[yy[index]][xx[index]].value = candidate;	// bingo
				return TRUE;
			}
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Function:	IsSolvable()
// Purpose:		Detects if a given puzzle is solvable.
//				How: Every row, column and 3x3block must have all numbers 1-9 alredy defined as value OR as a candidate.
// Parameters:
// Returns:		TRUE if the puzzle is solvable, otherwise FALSE.
/////////////////////////////////////////////////////////////////////////
BOOL SudSolver::IsSolvable(void)
{
	int x, y, value;
	BOOL bFound;
	static const int xx[81] = {0,1,2,0,1,2,0,1,2, 3,4,5,3,4,5,3,4,5, 6,7,8,6,7,8,6,7,8,   0,1,2,0,1,2,0,1,2, 3,4,5,3,4,5,3,4,5, 6,7,8,6,7,8,6,7,8,   0,1,2,0,1,2,0,1,2, 3,4,5,3,4,5,3,4,5, 6,7,8,6,7,8,6,7,8};
	static const int yy[81] = {0,0,0,1,1,1,2,2,2, 0,0,0,1,1,1,2,2,2, 0,0,0,1,1,1,2,2,2,   3,3,3,4,4,4,5,5,5, 3,3,3,4,4,4,5,5,5, 3,3,3,4,4,4,5,5,5,   6,6,6,7,7,7,8,8,8, 6,6,6,7,7,7,8,8,8, 6,6,6,7,7,7,8,8,8};

	// for each value or candidate
	for (value=1; value<10; value++)
	{	// for each 3x3 block
		for (y=0; y<9; y++)
		{	bFound = FALSE;
			int i = y*9;
			for (x=0; x<9; x++)
			{	if ((cell[yy[i]][xx[i]].value == value) ||
					(cell[yy[i]][xx[i]].value == 0 && cell[yy[i]][xx[i]].isCandidate[value] == TRUE))
				{	bFound = TRUE;
					break;
				}
				i++;
			}
			if (bFound == FALSE)
				return FALSE;
		}		
		// for each row
		for (y=0; y<9; y++)
		{	bFound = FALSE;
			for (x=0; x<9; x++)
			{	if ((cell[y][x].value == value) ||
					(cell[y][x].value == 0 && cell[y][x].isCandidate[value] == TRUE))
				{	bFound = TRUE;
					break;
			}	}
			if (bFound == FALSE)
				return FALSE;
		}
		// for each column
		for (x=0; x<9; x++)
		{	bFound = FALSE;
			for (y=0; y<9; y++)
			{	if ((cell[y][x].value == value) ||
					(cell[y][x].value == 0 && cell[y][x].isCandidate[value] == TRUE))
				{	bFound = TRUE;
					break;
			}	}
			if (bFound == FALSE)
				return FALSE;
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Function:	PrepareRecursionOrder()
// Purpose:		Initializes the m_cellOrder array with the coordinates.
// Parameters:
// Returns:
/////////////////////////////////////////////////////////////////////////
void SudSolver::PrepareRecursionOrder(void)
{
	// return if m_cellOrder is already initialized
	if (memcmp(&m_cellOrder[0], &m_cellOrder[1], sizeof(CellOrder)))
		return;

	// populate m_cellOrder array
	int i = 0;
	for (int y=0; y<9; y++)
	{	for (int x=0; x<9; x++)
		{	m_cellOrder[i].x = x;
			m_cellOrder[i].y = y;
			i++;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Function:	MixRecursionOrder()
// Purpose:		Randomly scrambles the m_cellOrder array with 
//				the hope that scrambled order will yield the brute-force solution faster.
// Parameters:
// Returns:
/////////////////////////////////////////////////////////////////////////
void SudSolver::MixRecursionOrder(void)
{
	int i, index1=0, index2;

	srand((unsigned)time(0));

	// find first empty cell in sequence order
	for (i=0; i<81; i++)
	{	if (cell[m_cellOrder[i].y][m_cellOrder[i].x].value == 0)
		{	index1 = i;
			break;
	}	}

	// swap first cell in m_cellOrder with a random one.
	// then swap 5 more random cells.
	for (i=0; i<6; i++)
	{	index2 = rand()%81;
		int x                 = m_cellOrder[index1].x;
		int y                 = m_cellOrder[index1].y;
		m_cellOrder[index1].x = m_cellOrder[index2].x;
		m_cellOrder[index1].y = m_cellOrder[index2].y;
		m_cellOrder[index2].x = x;
		m_cellOrder[index2].y = y;
		index1 = rand()%81;
	}
}

//////////////////////////////////////////////////////////////////////////
// Function:	Fixit()
// Purpose:		Corrects the result of OCR.
//				OCR must conform the sudoku valid values. If it doesn't, take the next best guess based on cell->OCR[] value.
//				FYI: This method is not reliable. You can comment it. Statistically, the program runs almost the same with or without it. This is because sometimes this method adds additional errors in the results, so the benefit is almost annulled with the introduced errors. The real benefit would be to raise the quality of OCR.
// Parameters:
// Returns:
/////////////////////////////////////////////////////////////////////////
void SudSolver::Fixit()
{
	for (int y=0; y<9; y++)
	{	for (int x=0; x<9; x++)
		{	if (cell[y][x].value != 0)
			{	SudSolverCell* pBad2 = GetWrongCell(y, x);
				if (pBad2 != NULL)	// if found a bad OCR value
				{	SudSolverCell* pBad1 = &cell[y][x];
					int vote1 = pBad1->OCR[pBad1->value];
					int vote2 = pBad2->OCR[pBad2->value];
					int diff1 = INT_MAX;
					int diff2 = INT_MAX;
					int newValue1 = pBad1->value;
					int newValue2 = pBad2->value;

					for (int i=1; i<10; i++)
						if (i != pBad1->value)
							if (vote1 < pBad1->OCR[i])
								if (pBad1->OCR[i] - vote1 < diff1)
								{	diff1 = pBad1->OCR[i] - pBad1->OCR[pBad1->value];
									newValue1 = i;
								}
					for (int i=1; i<10; i++)
						if (i != pBad2->value)
							if (vote2 < pBad2->OCR[i])
								if (pBad2->OCR[i] - pBad2->OCR[pBad2->value] < diff2)
								{	diff2 = pBad2->OCR[i] - vote2;
									newValue2 = i;
								}
					if (diff2 <= diff1)
					{	pBad2->OCR[pBad2->value] = INT_MAX;
						pBad2->value = newValue2;
					}
					else
					{	pBad1->OCR[pBad1->value] = INT_MAX;
						pBad1->value = newValue1;
	}	}	}	}	}
}

//////////////////////////////////////////////////////////////////////////
// Function:	GetWrongCell()
// Purpose:		Searches for a duplicate value in the same row, column and 3x3 block.
// Parameters:	celly, celly - input cell coordinate
// Returns:		pointer to conflicting cell of the input cell. NULL if there is no conflicting cell.
/////////////////////////////////////////////////////////////////////////
SudSolverCell* SudSolver::GetWrongCell(int celly, int cellx)
{	int x, y;
	static const int down[9] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
	static const int up[9]   = {2, 2, 2, 5, 5, 5, 8, 8, 8};
	int cellvalue = cell[celly][cellx].value;

	for (x=0; x<9; x++)							//  is duplicate in same row
		if (cell[celly][x].value == cellvalue)
			if (x != cellx)
				return &cell[celly][x];

	for (y=0; y<9; y++)							// is duplicate in same column
		if (cell[y][cellx].value == cellvalue)
			if (y != celly)
				return &cell[y][cellx];

	for (y=down[celly]; y<=up[celly]; y++)		// is duplicate in the same rect 3x3
		for (x=down[cellx]; x<=up[cellx]; x++)
			if (cell[y][x].value == cellvalue)
				if (x != cellx && y != celly)
					return &cell[y][x];
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// Function:	IsValidCell()
// Purpose:		Checks if the celly,cellx sudoku position has a valid value.
// Parameters:	celly, celly - input cell coordinate
// Returns:		TRUE if there is no duplicates for input cell, otherwise FALSE.
/////////////////////////////////////////////////////////////////////////
BOOL SudSolver::IsValidCell(int celly, int cellx)
{	
	return (GetWrongCell(celly, cellx) == NULL);
}

//////////////////////////////////////////////////////////////////////////
// Function:	GetSolution()
// Purpose:		Fills the result[] array.
// Parameters:	result pointer to array to fill.
// Returns:
/////////////////////////////////////////////////////////////////////////
void SudSolver::GetSolution(Solution *result)
{
	for (int x=0; x<9; x++)
		for (int y=0; y<9; y++)
			(*result)[y][x] = cell[y][x].value;
}

//////////////////////////////////////////////////////////////////////////
// Function:	BruteForce()
// Purpose:		Implementation of 'Brute force' aka 'trial and error' method.
//				Recurses thru all the cells with all the possible values (candidates).
// Parameters:	step - resursion step; One step = one cell.
// Returns:		TRUE if puzzle solved, otherwise FALSE.
/////////////////////////////////////////////////////////////////////////
BOOL SudSolver::BruteForce(int step)
{
	step++;
	if (step == 81)
		return TRUE;  // Recursion finished

	m_BruteForceStep++;
	if (m_BruteForceStep > MAXBRUTEFORCESTEP)	// Recursion takes too long
		return FALSE;

	// one step = one cell
	int x = m_cellOrder[step].x;
	int y = m_cellOrder[step].y;

	if (cell[y][x].value)		// skip init values
	{	if (BruteForce(step))	// go next cell
			return TRUE;
	}
	else
	{	for (int value=1; value<10; value++)				// try with all the values for the current cell
		{	if (cell[y][x].isCandidate[value] == FALSE)		// only candidates, please
				continue;
			cell[y][x].value = value;	// set the candidate value in the current cell
			if (IsValidCell(y, x))		// if this is a serious candidate
				if (BruteForce(step))	// go next cell
					return TRUE;
		}
		// if the possible value is NOT found
		cell[y][x].value = 0;			// reset the cell value
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Function:	SolveMe()
// Purpose:		Solves the sudoku puzzle.
// Parameters:
// Returns:		TRUE if puzzle solved, otherwise FALSE.
/////////////////////////////////////////////////////////////////////////
BOOL SudSolver::SolveMe(void)
{
	TRACE("\n---------");

	do {
		if (Step1() == TRUE)
		{	TRACE("\nsimple");
			return TRUE;			// sudoku solved
		}
	} while (Step2() == TRUE);


	// do not proceed if the cell values are invalid
	if (!IsSolvable())
		return FALSE;

	// if Step1 and Step2 did not find a final solution then fall back to 
	// brute-force method, which is slow but will find a solution no matter how difficult it is.

	BOOL ret = FALSE;
	PrepareRecursionOrder();
	for (int i=0; i<3; i++)		// 3 chances (retries) for brute-force method
	{
		m_BruteForceStep = 0;
		ret = BruteForce(-1);
		TRACE("\nBruteForceStep:%d", m_BruteForceStep);

		if (m_BruteForceStep < MAXBRUTEFORCESTEP)
			break;
		else
			MixRecursionOrder();
	}
	return ret;
}