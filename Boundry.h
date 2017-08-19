#pragma once
class CBoundry
{
public:
	CBoundry(void);
	~CBoundry(void);
	double Rset1;
	double Rset2;
	double Xset1;
	double Xset2;
	double Alpha;
	double Beta;
	double Theta;
	CString Name;
	CString Type;
	double defaultZPS;   //Z/ÏñËØ£¨Ä¬ÈÏ£©
	double ZPS;          //Z/ÏñËØ
	int isVisible;
	COLORREF Color;
	int drawBd(CDC* pDC, const CPoint& mid, const CRect& rect);

private:
	int drawPoly(CDC* pDC, const CPoint& mid);
	int drawLine(CDC* pDC, const CPoint& mid, const CRect& rect);
public:
	int drawCircle(CDC* pDC, const CPoint& mid);
	int drawRect(CDC* pDC, const CPoint& mid);
};

