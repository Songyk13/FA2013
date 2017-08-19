#include    "stdafx.h"
#include    "drawcube.h"


#define COLOR_WHITE RGB(255,255,255)
#define COLOR_BLACK RGB(0,0,0)
#define NCOLORSHADES 128		// this many shades in gradient

static void PaintRect(CDC* pDC, int x, int y, int w, int h, COLORREF color)
{
	CBrush brush(color);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->PatBlt(x, y, w, h, PATCOPY);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
}

void PaintGradiantRect(CDC *pDC, const RECT &rect,COLORREF clrFrom, COLORREF clrTo =RGB(255,255,255), 
                       BOOL hori = true, BOOL ascend = true)
{
	int cxCap = rect.right - rect.left;
	int cyCap = rect.bottom - rect.top;

	// Get the intensity values for the ending color
	int r1 = GetRValue(clrTo); // red
	int g1 = GetGValue(clrTo); // green
	int b1 = GetBValue(clrTo); // blue
	
	// Get the intensity values for the begining color
	int r2 = GetRValue(clrFrom); // red
	int g2 = GetGValue(clrFrom); // green
	int b2 = GetBValue(clrFrom); // blue

	int r, g, b;

    if(hori) //paint horizontal rect;
    {
	    int x = cxCap;	
	    int w = x;							// width of area to shade
	    int xDelta= max(w/NCOLORSHADES,1);	// width of one shade band


	    while (x >= xDelta) {
		    x -= xDelta;
		    if (r1 > r2)
			    r = r1 - (r1-r2)*(w-x)/w;
		    else
			    r = r1 + (r2-r1)*(w-x)/w;

		    if (g1 > g2)
			    g = g1 - (g1-g2)*(w-x)/w;
		    else
			    g = g1 + (g2-g1)*(w-x)/w;

		    if (b1 > b2)
			    b = b1 - (b1-b2)*(w-x)/w;
		    else
			    b = b1 + (b2-b1)*(w-x)/w;

            if(ascend) // Paint from  left to right;
		        PaintRect(pDC, rect.left+x, rect.top, xDelta, cyCap, RGB(r, g, b));
            else               // Paint from  right to left;
                PaintRect(pDC, rect.right-x-xDelta, rect.top, xDelta, cyCap, RGB(r, g, b));
	    }
    }
    else    //paint vertical rect;
    {
	    int y = cyCap;	
	    int w = y;							// height of area to shade
	    int yDelta= max(w/NCOLORSHADES,1);	// height of one shade band


	    //while (y >= yDelta) {
        while (y > 0) {
		    y -= yDelta;
		    if (r1 > r2)
			    r = r1 - (r1-r2)*(w-y)/w;
		    else
			    r = r1 + (r2-r1)*(w-y)/w;

		    if (g1 > g2)
			    g = g1 - (g1-g2)*(w-y)/w;
		    else
			    g = g1 + (g2-g1)*(w-y)/w;

		    if (b1 > b2)
			    b = b1 - (b1-b2)*(w-y)/w;
		    else
			    b = b1 + (b2-b1)*(w-y)/w;
		    
            if(ascend) // Paint from  top to bottom;
		        PaintRect(pDC, rect.left, rect.top+y, cxCap, yDelta, RGB(r, g, b));
            else       // Paint from  bottom to top;
                PaintRect(pDC, rect.left, rect.bottom-y-yDelta, cxCap, yDelta, RGB(r, g, b));
	    }
    }

}

void DrawColorCube(CDC* pDC, CRect& rect, COLORREF colorFrom, COLORREF colorTo /*= RGB(255,255,255)*/, 
                   BOOL IsHori /*= true*/)
{
    CRect       newRect;
    COLORREF    color;
    CPen*       oldPen;
    //CBrush*     oldBr;    

    /*
     *  If value as 0, draw a line here;
     */
    if(rect.Width()<=0 )
        rect.right = rect.left +2;
    else if(rect.Height() <=0)
        rect.top-=2;

    BOOL backHori = IsHori;
    IsHori = true;

    /* 
     * draw first 1/3 part;
     */
    if(IsHori)
    {
        newRect.SetRect(rect.left,rect.top,(rect.right - rect.left)/3+rect.left,rect.bottom);
        PaintGradiantRect(pDC,newRect,colorFrom, colorTo);
    }
    else
    {
        newRect.SetRect(rect.left,rect.bottom - (rect.bottom-rect.top)/3,rect.right,rect.bottom);
        PaintGradiantRect(pDC,newRect,colorFrom, colorTo, IsHori, false);
    }

    /*
     * then left 2/3 part;
     */
    if(IsHori)
    {
        newRect.SetRect((rect.right - rect.left)/3+rect.left,rect.top,rect.right,rect.bottom);
        PaintGradiantRect(pDC,newRect,colorFrom, colorTo, IsHori, false);
    }
    else
    {
        newRect.SetRect(rect.left,rect.top, rect.right, rect.bottom - (rect.bottom-rect.top)/3);
        PaintGradiantRect(pDC,newRect,colorFrom, colorTo, IsHori, true);
    }

 //   /*
 //    *   Frame the rect with colorFrom, with look better;
 //    */
 //   CPen pen(PS_SOLID,1,colorFrom);
 //   oldPen  = (CPen*)pDC->SelectObject(&pen);
 //   oldBr   = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
 //   //pDC->Rectangle(&rect);
 //   pDC->SelectObject(oldPen);
 //   pDC->SelectObject(oldBr);
	//pen.DeleteObject();

    /*
     *   Draw Shadow next;
     */
    if(backHori)//hori
    {
        color = RGB(170,170,170);
        CPen pen1(PS_SOLID,2,color);
        oldPen = (CPen*)pDC->SelectObject(&pen1);
        pDC->MoveTo(rect.left, rect.bottom+1);
        pDC->LineTo(rect.right+1, rect.bottom+1);
        pDC->LineTo(rect.right+1, rect.top);

        color = RGB(190,190,190);
        CPen pen2(PS_SOLID,2,color);
        pDC->SelectObject(&pen2);
        pDC->MoveTo(rect.left+1, rect.bottom+2);
        pDC->LineTo(rect.right+2, rect.bottom+2);
        pDC->LineTo(rect.right+2, rect.top+1);

		pDC->SelectObject(oldPen);
		pen1.DeleteObject();
		pen2.DeleteObject();
    }
    else//vert
    {
#if 0
        color = RGB(170,170,170);
        CPen pen1(PS_SOLID,1,color);
        oldPen = (CPen*)pDC->SelectObject(&pen1);
        pDC->MoveTo(rect.left, rect.top-2);
        pDC->LineTo(rect.right+1, rect.top-2);
        pDC->LineTo(rect.right+1, rect.bottom);

        color = RGB(190,190,190);
        CPen pen2(PS_SOLID,1,color);
        pDC->SelectObject(&pen2);
        pDC->MoveTo(rect.left+1, rect.top-3);
        pDC->LineTo(rect.right+1, rect.top-3);
        pDC->LineTo(rect.right+1, rect.bottom-1);

		pDC->SelectObject(oldPen);
		pen1.DeleteObject();
		pen2.DeleteObject();
#endif
    }

}


