#pragma once


#define MAXRESULTS 12
#define MAXCOUNTER 40

typedef int Solution[9][9];
struct SudResult
{
	Solution solution;
	unsigned int vote;
};


class SudResultVoter
{
public:
	SudResultVoter(void);
	~SudResultVoter(void);

	void Reset(void);
	void GetBestSolution(Solution *result);

private:
	SudResult m_solutions[MAXRESULTS];
	int m_Counter;
};
