#include "StdAfx.h"
#include "FA2013.h"
#include "Boundry.h"
#include <math.h>
#include "splab_vector.h"

using splab::PI;

CBoundry::CBoundry(void)
	: Rset1(0)
	, Rset2(0)
	, Xset1(0)
	, Xset2(0)
	, Alpha(0)
	, Beta(0)
	, Theta(0)
	, Name(_T(""))
	, Type(_T(""))
	, defaultZPS(0.5)
	, ZPS(0.5)
	, isVisible(0)
	, Color(0x00cdcdcd)
{
}


CBoundry::~CBoundry(void)
{
}


int CBoundry::drawBd(CDC* pDC, const CPoint& mid, const CRect& rect)
{
	CPen BdPen;
	BdPen.CreatePen(PS_SOLID,1,Color);
	CPen* pOld = pDC->SelectObject(&BdPen);
	switch(Type[0])
	{
	case _T('P'):
		{
			drawPoly(pDC, mid);
			break;
		}
	case _T('L'):
		{
			drawLine(pDC, mid, rect);
			break;
		}
	case _T('C'):
		{
			drawCircle(pDC, mid);
			break;
		}
	case _T('R'):
		{
			drawRect(pDC, mid);
			break;
		}
	}
	pDC->SelectObject(pOld);
	BdPen.DeleteObject();
	return 0;
}


int CBoundry::drawPoly(CDC* pDC, const CPoint& mid)
{
	CPoint A,B,C,D,O;
	O = mid;
	int x = mid.x;
	int y = mid.y;
	double xa,ya,xb,yb,xc,yc,xd,yd;
	double tA = Alpha/180*PI;
	double tT = Theta/180*PI;
	xa = Xset1/(Beta-1/tan(tA));
	ya = -xa*(1/tan(tA));
	xb = (Xset1+Rset1*tan(tT))/(tan(tT)+Beta);
	yb = (xb-Rset1)*tan(tT);
	xc = Rset1;
	yc = 0;
	xd = Rset1;
	yd = -Rset1*tan(tA);

	A.x = (int)(x + xa/ZPS);
	A.y = (int)(y - ya/ZPS);
	B.x = (int)(x + xb/ZPS);
	B.y = (int)(y - yb/ZPS);
	C.x = (int)(x + xc/ZPS);
	C.y = (int)(y - yc/ZPS);
	D.x = (int)(x + xd/ZPS);
	D.y = (int)(y - yd/ZPS);

	pDC->MoveTo(O);
	pDC->LineTo(A);
	pDC->MoveTo(A);
	pDC->LineTo(B);
	pDC->MoveTo(B);
	pDC->LineTo(C);
	pDC->MoveTo(C);
	pDC->LineTo(D);
	pDC->MoveTo(D);
	pDC->LineTo(O);

	return 0;
}


int CBoundry::drawLine(CDC* pDC, const CPoint& mid, const CRect& rect)
{
	CPoint A,B;
	double tB = Beta/180*PI;
	A.x = 0;
	A.y = (int)(mid.y-Xset1+(mid.x+Rset1) * tan(tB));
	B.x = rect.Width();
	B.y = mid.y-Xset1-(rect.Width()-mid.x-Rset1) * tan(tB);
	pDC->MoveTo(A);
	pDC->LineTo(B);
	return 0;
}


int CBoundry::drawCircle(CDC* pDC, const CPoint& mid)
{
	CRect Rect;
	CPoint s,e;
	double R = (1+Alpha)*sqrt(pow(Rset1,2)+pow(Xset1,2))/2/ZPS;
	Rect.left = (int)((1-Alpha)*Rset1/ZPS/2+mid.x-R);
	Rect.right = (int)((1-Alpha)*Rset1/ZPS/2+mid.x+R);
	Rect.top = (int)(-(1-Alpha)*Xset1/ZPS/2+mid.y-R);
	Rect.bottom = (int)(-(1-Alpha)*Xset1/ZPS/2+mid.y+R);
	s = Rect.TopLeft();
	e = s;
	pDC->Arc(Rect,s,e);
	return 0;
}


int CBoundry::drawRect(CDC* pDC, const CPoint& mid)
{
	CPoint A,B,C,D;
	A.x = mid.x+(int)(Rset2/ZPS);
	A.y = mid.y-(int)(Xset1/ZPS);
	B.x = mid.x+(int)(Rset1/ZPS);
	B.y = mid.y-(int)(Xset1/ZPS);
	C.x = mid.x+(int)(Rset1/ZPS);
	C.y = mid.y-(int)(Xset2/ZPS);
	D.x = mid.x+(int)(Rset2/ZPS);
	D.y = mid.y-(int)(Xset2/ZPS);
	pDC->MoveTo(D);
	pDC->LineTo(A);
	pDC->MoveTo(A);
	pDC->LineTo(B);
	pDC->MoveTo(B);
	pDC->LineTo(C);
	pDC->MoveTo(C);
	pDC->LineTo(D);
	return 0;
}
