// DView.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DView.h"

#include"FA2013Doc.h"


// CDView

IMPLEMENT_DYNCREATE(CDView, CScrollView)

CDView::CDView()
{

}

CDView::~CDView()
{
}


BEGIN_MESSAGE_MAP(CDView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_VSCROLL()
//	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDView 绘图

void CDView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	CFA2013Doc* pDoc = (CFA2013Doc*)GetDocument();
	pDoc->pDView=this;
}

void CDView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
	CFA2013Doc* pDoc1 =(CFA2013Doc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc1->pFile!=NULL)
	{
		CRect rt;
		this->GetClientRect(&rt);

		// 为屏幕DC创建兼容的内存DC
		CDC m_DC;
		if(!m_DC.CreateCompatibleDC(NULL))				
		{														
			::PostQuitMessage(0);
		}	
		this->drawCom(&m_DC);
	}
}


// CDView 诊断

#ifdef _DEBUG
void CDView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CDView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDView 消息处理程序


void CDView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	CFA2013Doc* pDoc = (CFA2013Doc*)GetDocument();
	if(pDoc->pFile!=NULL)
	{
		ScrollToPosition(CPoint(pDoc->chanPos.x,GetScrollPosition().y));
		//pDoc->UpdateAllViews(NULL);
		//SetScrollSizes(MM_TEXT,ComFile->viewSize);
		Invalidate();
		UpdateWindow();
	}

}


int CDView::drawCom(CDC* m_DC)
{
//	int mapMode;
	CSize totalSize;
	CSize size1, size2;
	CSize visibleSize;
	CPoint pPoint;
	CPoint startPoint;
	int i,j;
	int sumHeight = 0;
//	int currentHeight;
	int visibleA1, visibleA2;
	int visibleP;
	int startSample, endSample;
	
	extern CFA2013App theApp;
	CPen* pOld;

	CDC* pDC= this->GetDC();
	CRect bkgRect;
	this->GetClientRect(&bkgRect);

	CBitmap m_Bmp;
	m_Bmp.CreateCompatibleBitmap(pDC, bkgRect.Width(), bkgRect.Height());

	//::SelectObject(m_DC->GetSafeHdc(), m_Bmp);
	CBitmap* bOld = (CBitmap*)(m_DC->SelectObject(&m_Bmp));
//	this->GetDeviceScrollSizes(mapMode,totalSize,size1,size2);

	pPoint = this->GetScrollPosition();

                                                           //双缓冲
	//pDC->DPtoLP(&bkgRect);
	//pDC->FillSolidRect(bkgRect,RGB(255,255,255));

	//m_DC.DPtoLP(&bkgRect);
	m_DC->SetBkColor(RGB(0,0,0));
	m_DC->FillSolidRect(bkgRect,RGB(0,0,0));
	


	CFA2013Doc* ComFile = (CFA2013Doc*)GetDocument();
	totalSize.cx = (LONG)((ComFile->pFile->m_TotalSampleNum)*
		(ComFile->pFile->m_drawParam.SPScale))+1+ComFile->pFile->m_drawParam.leftSpace;
	totalSize.cy = (ComFile->pFile->m_DNumber+1)*(ComFile->pFile->m_drawParam.defaultDVSpace+2*(ComFile->pFile->m_drawParam.DGap));
	SetScrollSizes(MM_TEXT,totalSize);

	int visibleHeight = pPoint.y/*+ComFile->pFile->m_drawParam.topSpace*/;
	i = pPoint.y/(ComFile->pFile->m_drawParam.defaultDVSpace+2*(ComFile->pFile->m_drawParam.DGap))+1;
	j = (pPoint.y+bkgRect.Height())/(ComFile->pFile->m_drawParam.defaultDVSpace+2*(ComFile->pFile->m_drawParam.DGap))+1;
	startPoint.x=ComFile->pFile->m_drawParam.leftSpace;;
	startPoint.y = (ComFile->pFile->m_drawParam.defaultDVSpace+2*(ComFile->pFile->m_drawParam.DGap))*(i-1)-pPoint.y+ComFile->pFile->m_drawParam.DGap+ComFile->pFile->m_drawParam.defaultDVSpace/2;
	visibleA1 = i;
	visibleA2 = j;
	visibleP = bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace-ComFile->pFile->m_drawParam.leftSpace+1;
	//buchong
	
	startSample=(int)(pPoint.x/(ComFile->pFile->m_drawParam.SPScale));
	endSample=(int)((visibleP+pPoint.x)/(ComFile->pFile->m_drawParam.SPScale));
	if(startSample>ComFile->pFile->m_TotalSampleNum)
		startSample=ComFile->pFile->m_TotalSampleNum-1;
	if(endSample>ComFile->pFile->m_TotalSampleNum)
		endSample=ComFile->pFile->m_TotalSampleNum-1;
	m_DC->FillSolidRect((bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace),0,bkgRect.Width(),bkgRect.Height(),RGB(150,150,150));
	for(int ii = i;ii<=j&&ii<=ComFile->pFile->m_DNumber;++ii)
	{
		ComFile->pFile->DVector[ii-1]->drawChannel(m_DC,startPoint,startSample,endSample,ComFile->pFile->m_drawParam);
		//数值
		CString str1,str2;
		str1.Format(_T("%d"),ComFile->pFile->DVector[ii-1]->DData[ComFile->CursorNum1]);
		str2.Format(_T("%d"),ComFile->pFile->DVector[ii-1]->DData[ComFile->CursorNum2]);
		pOld = m_DC->SelectObject(&theApp.PenN);
		m_DC->MoveTo(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace+5,startPoint.y-ComFile->pFile->m_drawParam.defaultDVSpace/2);
		m_DC->LineTo(bkgRect.Width()-5,startPoint.y-ComFile->pFile->m_drawParam.defaultDVSpace/2);
		m_DC->MoveTo(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace+5,startPoint.y+ComFile->pFile->m_drawParam.defaultDVSpace/2);
		m_DC->LineTo(bkgRect.Width()-5,startPoint.y+ComFile->pFile->m_drawParam.defaultDVSpace/2);
		m_DC->MoveTo(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace/2,startPoint.y-ComFile->pFile->m_drawParam.defaultDVSpace/2);
		m_DC->LineTo(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace/2,startPoint.y+ComFile->pFile->m_drawParam.defaultDVSpace/2);
		m_DC->SelectObject(pOld);
		CFont* def_font = m_DC->SelectObject(&theApp.font);
		m_DC->SetTextColor(White);
		m_DC->SetBkMode(TRANSPARENT);
		m_DC->TextOutW(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace+15,startPoint.y-10,str1);
		m_DC->TextOutW(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace/2+15,startPoint.y-10,str2);
		m_DC->SelectObject(def_font);
		if(ii<ComFile->pFile->m_DNumber)
		{
			startPoint.y += (ComFile->pFile->m_drawParam.defaultDVSpace)+(ComFile->pFile->m_drawParam.DGap)*2;
		}
	}
	//m_DC->FillSolidRect((bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace),0,bkgRect.Width(),bkgRect.Height(),RGB(150,150,150));
	m_DC->FillSolidRect(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace,0,2,bkgRect.Height(),RGB(150,150,150));
	m_DC->FillSolidRect(0,0,ComFile->pFile->m_drawParam.leftSpace,bkgRect.Height(),RGB(150,150,150));
	////////////////光标/////////////////////////////
	if (ComFile->pCursor1.x!=0)
	{
		int x = ComFile->CursorNum1 * ComFile->pFile->m_drawParam.SPScale + ComFile->pFile->m_drawParam.leftSpace;
		int t_x = x-ComFile->chanPos.x;
		if(t_x >= ComFile->pFile->m_drawParam.leftSpace && t_x<=bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace)
		{
			pOld = m_DC->SelectObject(&theApp.PenAxis1);
			m_DC->MoveTo(t_x,0);
			m_DC->LineTo(t_x,bkgRect.Height());
			m_DC->SelectObject(pOld);
		}
		//ComFile->CursorNum1= (int)((pPoint.x + ComFile->pCursor1.x - ComFile->pFile->m_drawParam.leftSpace )/(ComFile->pFile->m_drawParam.SPScale));
	}
	if (ComFile->pCursor2.x!=0)
	{
		int x = ComFile->CursorNum2 * ComFile->pFile->m_drawParam.SPScale + ComFile->pFile->m_drawParam.leftSpace;
		int t_x = x-ComFile->chanPos.x;
		if(t_x >= ComFile->pFile->m_drawParam.leftSpace && t_x<=bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace)
		{
			pOld = m_DC->SelectObject(&theApp.PenAxis2);
			m_DC->MoveTo(t_x,0);
			m_DC->LineTo(t_x,bkgRect.Height());
			m_DC->SelectObject(pOld);
		}
		//ComFile->CursorNum2= (int)((pPoint.x + ComFile->pCursor2.x - ComFile->pFile->m_drawParam.leftSpace )/(ComFile->pFile->m_drawParam.SPScale));
	}
	//////////////////////////////////////////////////
	pDC->BitBlt(0, 0, bkgRect.Width(), bkgRect.Height(), m_DC, 0, 0, SRCCOPY);

	//Invalidate();
	m_DC->SelectObject(bOld);
	m_Bmp.DeleteObject();
	m_DC->DeleteDC();

	ReleaseDC(pDC);
	
	return 0;
}


BOOL CDView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->pFile!=NULL)
		return TRUE;
	else
		return CScrollView::OnEraseBkgnd(pDC);
}


void CDView::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSize sizeClient;
	CSize sizeSb;
	GetTrueClientSize(sizeClient, sizeSb);

	BOOL hasHorzBar(FALSE);
	BOOL hasVertBar(FALSE);
	CheckScrollBars(hasHorzBar,hasVertBar);
	if (hasHorzBar)
		lpncsp->rgrc[0].bottom += GetSystemMetrics(SM_CYHSCROLL);
	else
		lpncsp->rgrc[0].bottom += GetSystemMetrics(SM_CYHSCROLL)-sizeSb.cy;


	CScrollView::OnNcCalcSize(bCalcValidRects, lpncsp);
}


void CDView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	ASSERT_VALID(pDoc);
	if ((pDoc->pFile!=NULL) &&(pDoc->m_Cursor==CURSOR))
	{
		CDC* pDC=GetDC();
		CPoint pPoint = this->GetScrollPosition();
		CRect bkgRect;
		this->GetClientRect(&bkgRect);
		int x=point.x;
		if ((x>pDoc->pFile->m_drawParam.leftSpace)&&(x<bkgRect.Width()-pDoc->pFile->m_drawParam.rightSpace))
		{
			pDoc->CursorNum1= (int)((pPoint.x + point.x - pDoc->pFile->m_drawParam.leftSpace )/(pDoc->pFile->m_drawParam.SPScale));
			pDoc->pCursor1.x = (LONG)(pDoc->CursorNum1 * pDoc->pFile->m_drawParam.SPScale + pDoc->pFile->m_drawParam.leftSpace);// - pPoint.x;
		}
	ReleaseDC(pDC);
	}
	pDoc->UpdateAllViews(NULL);
	CScrollView::OnLButtonDown(nFlags, point);
}


void CDView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	ASSERT_VALID(pDoc);
	if  ((pDoc->pFile!=NULL) &&(pDoc->m_Cursor==CURSOR))
	{
		CDC* pDC=GetDC();
		CPoint pPoint = this->GetScrollPosition();
		CRect bkgRect;
		this->GetClientRect(&bkgRect);
		int x=point.x;
		if ((x>pDoc->pFile->m_drawParam.leftSpace)&&(x<bkgRect.Width()-pDoc->pFile->m_drawParam.rightSpace))
		{
			pDoc->CursorNum2= (int)((pPoint.x + point.x - pDoc->pFile->m_drawParam.leftSpace )/(pDoc->pFile->m_drawParam.SPScale));
			pDoc->pCursor2.x = (LONG)(pDoc->CursorNum2 * pDoc->pFile->m_drawParam.SPScale + pDoc->pFile->m_drawParam.leftSpace); //- pPoint.x;
		}
	ReleaseDC(pDC);
	}
	pDoc->UpdateAllViews(NULL);
	CScrollView::OnRButtonDown(nFlags, point);
}


void CDView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	pDoc->UpdateAllViews(NULL);
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

