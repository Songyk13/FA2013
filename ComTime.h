#pragma once
class CComTime
{
public:
	CComTime(void);
	~CComTime(void);
	friend CComTime operator+(const CComTime& time1, const int& offset);
	CComTime(CString Time1);
	CString PrintTimeWithDate(void);
	CString PrintTime(void);
protected:
	int yyyy;
	int mm;
	int dd;
	int h;
	int min;
	int s;
	int us;
};

