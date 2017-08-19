#include "FA2013Doc.h"
#include <vector>
class CFA2013Doc;
class CComtrade;

unsigned int ThreadFunc(LPVOID pParam)
{
	CFA2013Doc* pDoc = (CFA2013Doc*)pParam;
	int NUMA = pDoc->pFile->m_ANumber;
	int NUMDATA = pDoc->pFile->AVector[0]->AData.size();
	for (int i=0; i<NUMA; i++)
	{
		if (pDoc->exit==0)
		{
			pDoc->pThread=NULL;
			return 0;
		}
		pDoc->pFile->AVector[i]->AmpData.resize(NUMDATA);
		pDoc->pFile->AVector[i]->PhData.resize(NUMDATA);
		pDoc->ThreadPro = (i*100.0)/NUMA;
		for(int j=0; j<NUMDATA; j++)
		{
			if (pDoc->exit==0)
			{
				pDoc->pThread=NULL;
				return 0;
			}
			pDoc->pFile->AVector[i]->GetCurrentPhasor(j+1,pDoc->pFile->m_SampleRate);
			pDoc->pFile->AVector[i]->AmpData[j] = pDoc->pFile->AVector[i]->APhasor.Amplitude;
			pDoc->pFile->AVector[i]->PhData[j] = pDoc->pFile->AVector[i]->APhasor.phase;
		}
		pDoc->pFile->AVector[i]->ThreadFlag = 1;
	}
	pDoc->ThreadPro = 100.0;
	return 0;
}