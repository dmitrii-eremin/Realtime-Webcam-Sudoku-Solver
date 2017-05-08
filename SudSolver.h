#pragma once
#include "SudResultVoter.h"

#define MAXBRUTEFORCESTEP 600000

struct SudSolverCell
{
	int value;					// The cell recornition value itself. Could be inital or calculated.
	int OCR[10];				// OCR probability for each digit 1-9. Smaller value means bigger probability for that digit value.
	BOOL isCandidate[10];		// List of possible values for this cell. Ignored for initally set cells.
};

struct CellOrder				// order for brute force traverse
{
	int x;						// x coordinate of cell
	int y;						// y coordinate of cell
};

class SudSolver
{
public:
	SudSolver(void);
	~SudSolver(void);

	BOOL IsValidCell(int celly, int cellx);
	BOOL AreAllCellsValid(void);
	void Fixit(void);
	BOOL SolveMe(void);
	void GetSolution(Solution *result);

	SudSolverCell cell[9][9];			// sudoku grid values. If the cell is initally empty the value is 0.
private:
	SudSolverCell* GetWrongCell(int celly, int cellx);
	void PrepareRecursionOrder(void);
	void MixRecursionOrder(void);
	BOOL Step1(void);
	BOOL Step2(void);
	BOOL BruteForce(int step);
	BOOL IsSolvable(void);

	static CellOrder m_cellOrder[81];
	int m_BruteForceStep;
};
