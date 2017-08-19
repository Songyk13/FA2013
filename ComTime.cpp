#include "StdAfx.h"
#include "ComTime.h"


CComTime::CComTime(void)
{
}


CComTime::~CComTime(void)
{
}

CComTime::CComTime(CString Time1)
{
	int curPos=0;
	this->dd = _wtoi((Time1.Tokenize(_T("/,:."), curPos).GetBuffer()));
	this->mm = _wtoi((Time1.Tokenize(_T("/,:."), curPos).GetBuffer()));
	this->yyyy=_wtoi((Time1.Tokenize(_T("/,:."), curPos).GetBuffer()));
	this->h = _wtoi((Time1.Tokenize(_T("/,:."), curPos).GetBuffer()));
	this->min = _wtoi((Time1.Tokenize(_T("/,:."), curPos).GetBuffer()));
	this->s = _wtoi((Time1.Tokenize(_T("/,:."), curPos).GetBuffer()));
	this->us = _wtoi((Time1.Tokenize(_T("/,:."), curPos).GetBuffer()));
}

CString CComTime::PrintTimeWithDate(void)
{
	CString YYYY,MM,DD,H,MIN,S,US,TOTAL;
	YYYY.Format(_T("%d"),this->yyyy);
	MM.Format(_T("%d"),this->mm);
	DD.Format(_T("%d"),this->dd);
	H.Format(_T("%d"),this->h);
	MIN.Format(_T("%d"),this->min);
	S.Format(_T("%d"),this->s);
	US.Format(_T("%d"),this->us);
	TOTAL = YYYY + _T("年") + MM + _T("月") + DD + _T("日") + H + _T("时") + MIN + _T("分") + S + _T("秒") + US +_T("微秒");
	return TOTAL;
}

CString CComTime::PrintTime(void)
{
	CString H,MIN,S,US,TOTAL;
	H.Format(_T("%d"),this->h);
	MIN.Format(_T("%d"),this->min);
	S.Format(_T("%d"),this->s);
	US.Format(_T("%d"),this->us);
	TOTAL = H + _T(":") + MIN + _T(":") + S + _T(":") + US +_T("");
	return TOTAL;
}

CComTime operator+(const CComTime& time1, const int& offset)
{
	CComTime time2 = time1;
	int i=0;    //进位 
	if (offset>=0)
	{
		time2.us += offset;
		if (time2.us>=1000000)
		{
			i= (int)(time2.us/1000000);
			time2.us  = time2.us % 1000000;
			time2.s += i;
		}
		if (time2.s>=60)
		{
			i= (int)(time2.s/60);
			time2.s  = time2.s % 60;
			time2.min += i;
		}
		if (time2.min>=60)
		{
			i= (int)(time2.min/60);
			time2.min  = time2.min % 60;
			time2.h += i;
		}
		if (time2.h>=24)
		{
			i= (int)(time2.h/24);
			time2.h  = time2.h % 24;
		}
		if ((time2.dd == 28)&&(time2.mm == 2))
		{
			if (((time2.yyyy % 4 ==0)&&(time2.yyyy % 100!=0))||(time2.yyyy % 400 == 0))    //闰年
			{	
				time2.dd +=i;
				i = (int)time2.dd / 29;
				time2.dd = time2.dd % 29; 

			}
			else    //平年
			{
				time2.dd +=i;
				i = (int)time2.dd / 28;
				time2.dd = time2.dd % 28; 
			}
		}
		else if ((time2.dd == 30)&&((time2.mm ==4)||(time2.mm ==6)||(time2.mm ==9)||(time2.mm ==11)))
		{
			time2.dd +=i;
			i = (int)time2.dd / 30;
			time2.dd = time2.dd % 30; 
		}
		else
		{
			time2.dd +=i;
			i = (int)time2.dd / 31;
			time2.dd = time2.dd % 31; 
		}
		time2.mm +=i;
		if (time2.mm>=12)
		{
			i= (int)(time2.mm/12);
			time2.mm  = time2.mm % 12;
			time2.yyyy += i;
		}
	}
	else
	{
		time2.us += offset;
		if (time2.us<0)
		{
			i= (int)((-time2.us)/1000000)+1;
			time2.us  = 1000000 + (time2.us % 1000000);
			time2.s -= i;
		}
		if (time2.s<0)
		{
			i= (int)(-time2.s/60)+1;
			time2.s  = 60 + (time2.s % 60);
			time2.min -= i;
		}
		if (time2.min<0)
		{
			i= (int)(-time2.min/60)+1;
			time2.min  = 60 + (time2.min % 60);
			time2.h -= i;
		}
		if (time2.h<0)
		{
			i= (int)(-time2.h/24)+1;
			time2.h  = 24 + (time2.h % 24);
			//time2.dd += i;
		}
		if ((time2.dd == 1)&&(time2.mm == 3))
		{
			if (((time2.yyyy % 4 ==0)&&(time2.yyyy % 100!=0))||(time2.yyyy % 400 == 0))    //闰年
			{	
				time2.dd -= i;
				i = (int)(-time2.dd / 29)+1;
				time2.dd = 29 + (time2.dd % 29); 

			}
			else    //平年
			{
				time2.dd -= i;
				i = (int)(-time2.dd / 28)+1;
				time2.dd = 28 + (time2.dd % 28); 
			}
		}
		else if ((time2.dd == 1)&&((time2.mm ==5)||(time2.mm ==7)||(time2.mm ==10)||(time2.mm ==12)))
		{
			time2.dd -= i;
			i = (int)(-time2.dd / 30)+1;
			time2.dd = 30 + (time2.dd % 30); 
		}
		else if (time2.dd == 1)
		{
			time2.dd -= i;
			i = (int)( -time2.dd / 31)+1;
			time2.dd = 31+ (time2.dd % 31); 
		}
		else
		{
			time2.dd -= i;
			i=0;
		}
		time2.mm -=i;
		if (time2.mm<1)
		{
			time2.mm  = 12 - time2.mm;
			time2.yyyy--;
		}

	}
	return time2;
}
