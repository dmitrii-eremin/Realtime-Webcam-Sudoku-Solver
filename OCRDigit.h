#pragma once

#include <stdio.h>
#include "SudSolver.h"


struct RatioWH
{
	int ratio;
	int count;
};



class OCRDigit
{
public:
	OCRDigit(void);
	~OCRDigit(void);

	void Train(void);
	void Classify(const BYTE* blob, const POINT& lb, const POINT& rt, SudSolverCell* cell);

private:
	void Train(CString filename, int digit, int (*zon)[5][5], RatioWH *ratio);
	
	static const int m_zon[9][5][5];
	static const int m_ratios[9];
};
