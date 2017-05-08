#include "stdafx.h"
#include "SudResultVoter.h"


SudResultVoter::SudResultVoter(void)
{	ZeroMemory(m_solutions, sizeof(m_solutions));
	m_Counter = 0;
}

SudResultVoter::~SudResultVoter(void)
{}

void SudResultVoter::Reset()
{
	m_Counter = 0;
	for (int i=0; i<MAXRESULTS; i++)
		m_solutions[i].vote = m_solutions[i].vote / (MAXCOUNTER/10);	// don't reset the existing values to zero. Just make them lower.
}

void SudResultVoter::GetBestSolution(Solution *result)
{
	int i, minIndex=0, maxIndex=0;
	unsigned int minVote = UINT_MAX;
	unsigned int maxVote = 0;
	BOOL bFound = FALSE;

	m_Counter++;
	if (m_Counter > MAXCOUNTER)
		Reset();

	// search if input solution already exists in m_solutions
	for (i=0; i<MAXRESULTS; i++)
	{
		if (memcmp(result, m_solutions[i].solution, sizeof(Solution)) == 0)
		{	bFound = TRUE;
			m_solutions[i].vote += 1;
			break;
		}
		
		// find worst solution index
		if (minVote > m_solutions[i].vote)
		{	minVote = m_solutions[i].vote;
			minIndex = i;
		}
	}

	// if solution doesn't exist in m_solutions insert it. Overwrite the worst solution.
	if (bFound == FALSE)
	{	memcpy(m_solutions[minIndex].solution, result, sizeof(Solution));
		m_solutions[minIndex].vote = 1;
	}
	// find the best solution
	for (i=0; i<MAXRESULTS; i++)
	{	if (maxVote < m_solutions[i].vote)
		{	maxVote = m_solutions[i].vote;
			maxIndex = i;
		}
	}
	// Overwrite the passed solution with the best solution
	memcpy(result, m_solutions[maxIndex].solution, sizeof(Solution));
}

