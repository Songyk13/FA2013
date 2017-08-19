
// AView.cpp : CAView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "FA2013.h"
#endif

#include "FA2013Doc.h"
#include "AView.h"
#include "DView.h"
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAView

IMPLEMENT_DYNCREATE(CAView, CScrollView)

BEGIN_MESSAGE_MAP(CAView, CScrollView)
	// 标准打印命令
//	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAView::OnFilePrintPreview)
//	ON_WM_CONTEXTMENU()
//	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
//	ON_WM_SETCURSOR()
ON_COMMAND(ID_RESET, &CAView::OnReset)
ON_WM_VSCROLL()
ON_WM_HSCROLL()
ON_WM_KEYDOWN()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CAView 构造/析构

CAView::CAView()
{
	// TODO: 在此处添加构造代码

}

CAView::~CAView()
{
}

BOOL CAView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CAView 绘制

void CAView::OnDraw(CDC* /*pDC*/)
{
	CFA2013Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->pFile!=NULL)
	{
		// 为屏幕DC创建兼容的内存DC
		CDC m_DC;
		if(!m_DC.CreateCompatibleDC(NULL))				
		{														
			::PostQuitMessage(0);
		}
		this->drawCom(&m_DC);
		m_DC.DeleteDC();
	}
}

void CAView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = 1000;
	sizeTotal.cy = 1000;
	SetScrollSizes(MM_TEXT, sizeTotal,CSize(100,100),CSize(100,100));
	CFA2013Doc* pDoc = (CFA2013Doc*)GetDocument();
	pDoc->pAView=this;

}


// CAView 打印


//void CAView::OnFilePrintPreview()
//{
//#ifndef SHARED_HANDLERS
//	AFXPrintPreview(this);
//#endif
//}

BOOL CAView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CAView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CAView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

//void CAView::OnRButtonUp(UINT /* nFlags */, CPoint point)
//{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}

//void CAView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
//}


// CAView 诊断

#ifdef _DEBUG
void CAView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFA2013Doc* CAView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFA2013Doc)));
	return (CFA2013Doc*)m_pDocument;
}
#endif //_DEBUG


// CAView 消息处理程序

int CAView::drawCom(CDC* m_DC)
{

	//SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursorW(NULL,(LPCWSTR)_T("Cross.cur")));
	//HCURSOR cur = AfxGetApp()->LoadCursorW(IDC_POINTER);
	//HANDLE cur=LoadImage(NULL,TEXT("Cross1.cur"),IMAGE_CURSOR,32,32,LR_LOADFROMFILE);
	
	//SetCursor((HCURSOR)cur);
	
//	int mapMode;
	CSize totalSize;
	CSize size1, size2;
	CSize visibleSize;
	CPoint pPoint;
	CPoint startPoint;
	int i,j;
	int sumHeight = 0;
	int currentHeight;
	int visibleA1, visibleA2;
	int visibleP;
	int startSample, endSample;

	CPen* pOld;
	extern CFA2013App theApp;
	CRect bkgRect;
	this->GetClientRect(&bkgRect);

	CDC* pDC= this->GetDC();
	CBitmap m_Bmp;
	m_Bmp.CreateCompatibleBitmap(pDC, bkgRect.Width(), bkgRect.Height());

	//SelectObject(m_DC->GetSafeHdc(), m_Bmp);
	CBitmap* bOld = (CBitmap*)(m_DC->SelectObject(&m_Bmp));
//	this->GetDeviceScrollSizes(mapMode,totalSize,size1,size2);

	pPoint = this->GetScrollPosition();

                                                           //双缓冲
	//pDC->DPtoLP(&bkgRect);
	//pDC->FillSolidRect(bkgRect,RGB(255,255,255));

	//m_DC->DPtoLP(&bkgRect);
	m_DC->SetBkColor(RGB(0,0,0));
	//m_DC->FillSolidRect(bkgRect,RGB(0,0,0));
	


	CFA2013Doc* ComFile = (CFA2013Doc*)GetDocument();
	totalSize.cx = (LONG)((ComFile->pFile->m_TotalSampleNum)*
		(ComFile->pFile->m_drawParam.SPScale))+ComFile->pFile->m_drawParam.rightSpace
		+ComFile->pFile->m_drawParam.leftSpace;
	if(totalSize.cx<bkgRect.Width())
	{
		totalSize.cx=bkgRect.Width();
	}
	//totalSize.cy = (ComFile->pFile->m_ANumber+1)*(ComFile->pFile->m_drawParam.defaultVSpace+2*(ComFile->pFile->m_drawParam.VGap));
	totalSize.cy = ComFile->pFile->getTotalAVSpace()+ComFile->pFile->m_drawParam.VGap;
	SetScrollSizes(MM_TEXT,totalSize);
	ComFile->viewSize=totalSize;
//	ComFile->pDView->SetScrollSizes(MM_TEXT,totalSize);

	int visibleHeight = pPoint.y/*+ComFile->pFile->m_drawParam.topSpace*/;
	for(i = 0;sumHeight<=visibleHeight;++i)
	{
		sumHeight += ComFile->pFile->AVector[i]->m_drawPara.VSpace+ComFile->pFile->m_drawParam.VGap;
	}

	visibleHeight += bkgRect.Height()-ComFile->pFile->m_drawParam.topSpace;
	currentHeight = sumHeight-(ComFile->pFile->AVector[i-1]->m_drawPara.VSpace/2+(ComFile->pFile->m_drawParam.VGap));
	sumHeight -= ComFile->pFile->AVector[i]->m_drawPara.VSpace+(ComFile->pFile->m_drawParam.VGap);
	startPoint.x=ComFile->pFile->m_drawParam.leftSpace;
	startPoint.y=currentHeight-pPoint.y+ComFile->pFile->m_drawParam.topSpace+ComFile->pFile->m_drawParam.VGap;


	for(j = i+1;sumHeight<visibleHeight&&j<ComFile->pFile->m_ANumber;++j)
	{
		sumHeight += ComFile->pFile->AVector[j]->m_drawPara.VSpace+(ComFile->pFile->m_drawParam.VGap);
	}
	visibleA1 = i;
	visibleA2 = j;
	visibleP = bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace-ComFile->pFile->m_drawParam.leftSpace+1;
	//补充：

	startSample=(int)(pPoint.x/(ComFile->pFile->m_drawParam.SPScale));
	endSample=(int)((visibleP+pPoint.x)/(ComFile->pFile->m_drawParam.SPScale));
	if(startSample>ComFile->pFile->m_TotalSampleNum)
		startSample=ComFile->pFile->m_TotalSampleNum-1;
	if(endSample>ComFile->pFile->m_TotalSampleNum)
		endSample=ComFile->pFile->m_TotalSampleNum-1;
	m_DC->FillSolidRect((bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace),0,ComFile->pFile->m_drawParam.rightSpace,bkgRect.Height(),RGB(150,150,150));
	m_DC->FillSolidRect(0,0,ComFile->pFile->m_drawParam.leftSpace,bkgRect.Height(),RGB(150,150,150));
	for(int ii = i;ii<=j&&ii<=ComFile->pFile->m_ANumber;++ii)
	{
		//波形
		ComFile->pFile->AVector[ii-1]->drawChannel(m_DC,startPoint,startSample,endSample,ComFile->pFile->m_drawParam);
		if (ii == ComFile->ChanNum1+1)
		{
			pOld = m_DC->SelectObject(&theApp.PenCur);
			m_DC->MoveTo(startPoint.x,startPoint.y+ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace/2+1);
			m_DC->LineTo((int)(startPoint.x+(endSample-startSample-1)*ComFile->pFile->m_drawParam.SPScale),(int)(startPoint.y+ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace/2+1));
			m_DC->MoveTo((int)(startPoint.x+(endSample-startSample-1)*ComFile->pFile->m_drawParam.SPScale),(int)(startPoint.y-ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace/2-1));
			m_DC->LineTo(startPoint.x,startPoint.y-ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace/2-1);
			m_DC->SelectObject(pOld);
		}
		//数值
		CString str1,str2;
		str1.Format(_T("%.3f"),ComFile->pFile->AVector[ii-1]->AData[ComFile->CursorNum1]);
		str2.Format(_T("%.3f"),ComFile->pFile->AVector[ii-1]->AData[ComFile->CursorNum2]);
		pOld = m_DC->SelectObject(&theApp.PenN);
		m_DC->MoveTo(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace+5,startPoint.y-ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace/2);
		m_DC->LineTo(bkgRect.Width()-5,startPoint.y-ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace/2);
		m_DC->MoveTo(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace+5,startPoint.y+ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace/2);
		m_DC->LineTo(bkgRect.Width()-5,startPoint.y+ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace/2);
		m_DC->MoveTo(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace/2,startPoint.y-ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace/2);
		m_DC->LineTo(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace/2,startPoint.y+ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace/2);
		m_DC->SelectObject(pOld);
		CFont* def_font = m_DC->SelectObject(&theApp.font);
		m_DC->SetTextColor(White);
		m_DC->SetBkMode(TRANSPARENT);
		m_DC->TextOutW(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace+15,startPoint.y-10,str1);
		m_DC->TextOutW(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace/2+15,startPoint.y-10,str2);
		m_DC->SelectObject(def_font);
		if(ii<ComFile->pFile->m_ANumber)
		{
			startPoint.y += (ComFile->pFile->AVector[ii-1]->m_drawPara.VSpace)/2+(ComFile->pFile->m_drawParam.VGap)
				+(ComFile->pFile->AVector[ii]->m_drawPara.VSpace)/2;
		}
	}
	//pDC->FillSolidRect(0,0,bkgRect.Width(),100,RGB(150,150,150));

	m_DC->FillSolidRect(0,0,bkgRect.Width(),ComFile->pFile->m_drawParam.topSpace,RGB(150,150,150));
    m_DC->FillSolidRect(bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace,0,2,bkgRect.Height(),RGB(150,150,150));


	startPoint.x=ComFile->pFile->m_drawParam.leftSpace;
	startPoint.y=ComFile->pFile->m_drawParam.topSpace;
	drawAxis(m_DC,startPoint,startSample,endSample,ComFile->pFile);

	ComFile->chanPos=GetScrollPosition();
	////////////光标//////////////////////
	if (ComFile->pCursor1.x!=0)
	{
		int x = ComFile->CursorNum1 * ComFile->pFile->m_drawParam.SPScale + ComFile->pFile->m_drawParam.leftSpace;
		int t_x = x-ComFile->chanPos.x;
		if (t_x >= ComFile->pFile->m_drawParam.leftSpace && t_x<=bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace)
		{
			pOld = m_DC->SelectObject(&theApp.PenAxis1);
			m_DC->MoveTo(t_x,ComFile->pFile->m_drawParam.topSpace);
			m_DC->LineTo(t_x,bkgRect.Height());
			m_DC->SelectObject(pOld);
		}
		//ComFile->CursorNum1= (int)((pPoint.x + ComFile->pCursor1.x - ComFile->pFile->m_drawParam.leftSpace )/(ComFile->pFile->m_drawParam.SPScale));
		//m_DC->SetTextColor(RGB(255,255,255));
		//m_DC->SetBkMode(TRANSPARENT);
		//CString a,b,at;
		//CComTime aT;
		//aT = ComFile->pFile->m_StartTime + ComFile->pFile->Time[ComFile->CursorNum1];
		//a.Format(_T("%d"),ComFile->CursorNum1);
		//b.Format(_T("%d"),ComFile->pCursor1.x);
		//at = aT.PrintTimeWithDate();
		//m_DC->TextOutW(0,0,a);
		//m_DC->TextOutW(0,20,b);
		//m_DC->TextOutW(50,0,at);
	}
	if (ComFile->pCursor2.x!=0)
	{
		int x = ComFile->CursorNum2 * ComFile->pFile->m_drawParam.SPScale + ComFile->pFile->m_drawParam.leftSpace;
		int t_x = x-ComFile->chanPos.x;
		if (t_x >= ComFile->pFile->m_drawParam.leftSpace && t_x<=bkgRect.Width()-ComFile->pFile->m_drawParam.rightSpace)
		{
			pOld = m_DC->SelectObject(&theApp.PenAxis2);
			m_DC->MoveTo(t_x,ComFile->pFile->m_drawParam.topSpace);
			m_DC->LineTo(t_x,bkgRect.Height());
			m_DC->SelectObject(pOld);
		}
	}
	/////////////////////////////////////////////////
	pDC->BitBlt(0, 0, bkgRect.Width(), bkgRect.Height(), m_DC, 0, 0, SRCCOPY);


	//Invalidate();

	//ComFile->UpdateAllViews(NULL);
	m_DC->SelectObject(bOld);
	m_Bmp.DeleteObject();
	m_DC->DeleteDC();
	ReleaseDC(pDC);
	ComFile->pDView->Invalidate();
	ComFile->UpdateAllViews(this);
	return 0;
}



BOOL CAView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->pFile!=NULL)
		return TRUE;
	else
		return CScrollView::OnEraseBkgnd(pDC);
	//return CScrollView::OnEraseBkgnd(pDC);
}


void CAView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->pFile==NULL)
		return;

	CDC* pDC=GetDC();
	CPoint pPoint = this->GetScrollPosition();
	CRect bkgRect;
	this->GetClientRect(&bkgRect);
	pDoc->ChanNum1=pDoc->pFile->getPointChan(GetScrollPosition().y,point.y);
	if (pDoc->m_Cursor==CURSOR)
	{
		int x=point.x;
		if ((x>pDoc->pFile->m_drawParam.leftSpace)&&(x<bkgRect.Width()-pDoc->pFile->m_drawParam.rightSpace)
			&&x<=(pDoc->pFile->m_TotalSampleNum)*(pDoc->pFile->m_drawParam.SPScale))
		{
			pDoc->CursorNum1= (int)((pPoint.x + point.x - pDoc->pFile->m_drawParam.leftSpace )/(pDoc->pFile->m_drawParam.SPScale))-1;
			pDoc->pCursor1.x = (int)(pDoc->CursorNum1 * pDoc->pFile->m_drawParam.SPScale + pDoc->pFile->m_drawParam.leftSpace);
			pDoc->ChanNum1=pDoc->pFile->getPointChan(GetScrollPosition().y,point.y);
			if(pDoc->pDlgForm!=NULL)
			{
				if ( pDoc->pDlgForm->IsWindowVisible() == 1)
				{
					pDoc->pDlgForm->AList.SetItemState(pDoc->ChanNum1,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
					pDoc->pDlgForm->AList.EnsureVisible(pDoc->ChanNum1,FALSE);
					pDoc->pDlgForm->AList.SetSelectionMark(pDoc->ChanNum1);
					pDoc->pDlgForm->AList.SetFocus();
				}
			}
			if(pDoc->pDlgHarmo!=NULL)
			{
				if ( pDoc->pDlgHarmo->IsWindowVisible() == 1)
				{
					pDoc->pDlgHarmo->m_pView->Invalidate();
					pDoc->pDlgHarmo->m_pView->UpdateWindow();
					pDoc->pDlgHarmo->Alist.SetCurSel(pDoc->ChanNum1);
				}
			}
			if(pDoc->pDlgPhase!=NULL)
			{
				if ( pDoc->pDlgPhase->IsWindowVisible() == 1)
				{
					pDoc->pDlgPhase->m_pView->Invalidate();
					pDoc->pDlgPhase->m_pView->UpdateWindow();
				}
			}
			if(pDoc->pDlgSeqComp!=NULL)
			{
				if ( pDoc->pDlgSeqComp->IsWindowVisible() == 1)
				{
					pDoc->pDlgSeqComp->m_pPView->Invalidate();
					pDoc->pDlgSeqComp->m_pSView->Invalidate();
					pDoc->pDlgSeqComp->m_pPView->UpdateWindow();
					pDoc->pDlgSeqComp->m_pSView->UpdateWindow();
				}
			}
			if(pDoc->pDlgImp!=NULL)
			{
				if ( pDoc->pDlgImp->IsWindowVisible() == 1)
				{
					pDoc->pDlgImp->m_pView->Invalidate();
					pDoc->pDlgImp->m_pView->UpdateWindow();
				}
			}
			if(pDoc->pDlgBusDiff!=NULL)
			{
				if ( pDoc->pDlgBusDiff->IsWindowVisible() == 1)
				{
					pDoc->pDlgBusDiff->m_pView->Invalidate();
					pDoc->pDlgBusDiff->m_pView->UpdateWindow();
				}
			}

		}
	}
	else if(pDoc->m_Cursor==VZOOM)
	{
		if (point.x<pDoc->pFile->m_drawParam.leftSpace||point.x>bkgRect.Width()-pDoc->pFile->m_drawParam.rightSpace||point.y<pDoc->pFile->m_drawParam.topSpace)
			return;
		int i = pDoc->pFile->getPointChan(GetScrollPosition().y,point.y);
		pDoc->pFile->AVector[i]->m_drawPara.VSpace += 10;
		pDoc->pFile->AVector[i]->m_drawPara.UPScale = pDoc->pFile->AVector[i]->m_drawPara.UPScale*
			(pDoc->pFile->AVector[i]->m_drawPara.VSpace-10)/pDoc->pFile->AVector[i]->m_drawPara.VSpace;
	}
	else if(pDoc->m_Cursor==HZOOM)
	{
		pDoc->pFile->m_drawParam.LastSPScale = pDoc->pFile->m_drawParam.SPScale;
		if (point.x<pDoc->pFile->m_drawParam.leftSpace||point.x>bkgRect.Width()-pDoc->pFile->m_drawParam.rightSpace||point.y<pDoc->pFile->m_drawParam.topSpace)
			return;
		if (pDoc->pFile->m_drawParam.SPScale * 2 > pDoc->pFile->m_drawParam.defaultSPScale*16)
			return;
		pDoc->pFile->m_drawParam.SPScale *= 2;
	}
	ReleaseDC(pDC);
//	Invalidate();
	//UpdateWindow();
	pDoc->UpdateAllViews(NULL);
	CScrollView::OnLButtonDown(nFlags, point);
}


void CAView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->pFile==NULL)
		return;

	CDC* pDC=GetDC();
	CPoint pPoint = this->GetScrollPosition();
	CRect bkgRect;
	this->GetClientRect(&bkgRect);

	if(pDoc->m_Cursor==CURSOR)
	{
		int x=point.x;
		if ((x>pDoc->pFile->m_drawParam.leftSpace)&&(x<bkgRect.Width()-pDoc->pFile->m_drawParam.rightSpace)
			&&x<=(pDoc->pFile->m_TotalSampleNum)*(pDoc->pFile->m_drawParam.SPScale))
		{
			pDoc->CursorNum2= (int)((pPoint.x + point.x - pDoc->pFile->m_drawParam.leftSpace )/(pDoc->pFile->m_drawParam.SPScale));
			pDoc->pCursor2.x = (LONG)(pDoc->CursorNum2 * pDoc->pFile->m_drawParam.SPScale + pDoc->pFile->m_drawParam.leftSpace); //- pPoint.x;
		}
	}
	else if(pDoc->m_Cursor==VZOOM)
	{
		if (point.x<pDoc->pFile->m_drawParam.leftSpace||point.x>bkgRect.Width()-pDoc->pFile->m_drawParam.rightSpace||point.y<pDoc->pFile->m_drawParam.topSpace)
			return;
		int i = pDoc->pFile->getPointChan(GetScrollPosition().y,point.y);
		if (pDoc->pFile->AVector[i]->m_drawPara.VSpace==pDoc->pFile->AVector[i]->m_drawPara.defultVSpace)
			return;
		pDoc->pFile->AVector[i]->m_drawPara.VSpace -= 10;
		pDoc->pFile->AVector[i]->m_drawPara.UPScale = pDoc->pFile->AVector[i]->m_drawPara.UPScale*
			(pDoc->pFile->AVector[i]->m_drawPara.VSpace+10)/pDoc->pFile->AVector[i]->m_drawPara.VSpace;
	}
	else if(pDoc->m_Cursor==HZOOM)
	{
		if (point.x<pDoc->pFile->m_drawParam.leftSpace||point.x>bkgRect.Width()-pDoc->pFile->m_drawParam.rightSpace||point.y<pDoc->pFile->m_drawParam.topSpace)
			return;
		if (pDoc->pFile->m_drawParam.SPScale * 0.5 <=  pDoc->pFile->m_drawParam.defaultSPScale/32)
		{
			pDoc->pFile->m_drawParam.LastSPScale = pDoc->pFile->m_drawParam.SPScale;
			pDoc->pFile->m_drawParam.SPScale = pDoc->pFile->m_drawParam.defaultSPScale/32;
		}
		else
		{
			pDoc->pFile->m_drawParam.LastSPScale = pDoc->pFile->m_drawParam.SPScale;
			pDoc->pFile->m_drawParam.SPScale *= 0.5;
		}
	}
	ReleaseDC(pDC);
	//Invalidate();
//	UpdateWindow();
	pDoc->UpdateAllViews(NULL);
	CScrollView::OnRButtonDown(nFlags, point);
}


void CAView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CScrollView::OnLButtonUp(nFlags, point);
}


void CAView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	ASSERT_VALID(pDoc);
	CRect bkgRect;
		this->GetClientRect(&bkgRect);
	if (pDoc->pFile==NULL||point.x<pDoc->pFile->m_drawParam.leftSpace||
		point.x>bkgRect.Width()-pDoc->pFile->m_drawParam.rightSpace||point.y<pDoc->pFile->m_drawParam.topSpace)
		return;
	if(pDoc->m_Cursor==VZOOM)
	{
		int i = pDoc->pFile->getPointChan(GetScrollPosition().y,point.y);
		pDoc->pFile->AVector[i]->m_drawPara.VSpace -= 10;
		pDoc->pFile->AVector[i]->m_drawPara.UPScale = 2*pDoc->pFile->AVector[i]->maxAData/pDoc->pFile->AVector[i]->m_drawPara.VSpace;
	}
	Invalidate();
	UpdateWindow();
	CScrollView::OnLButtonDblClk(nFlags, point);
}


void CAView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	ASSERT_VALID(pDoc);
	CRect bkgRect;
		this->GetClientRect(&bkgRect);
	if (pDoc->pFile==NULL||point.x<pDoc->pFile->m_drawParam.leftSpace||
		point.x>bkgRect.Width()-pDoc->pFile->m_drawParam.rightSpace||point.y<pDoc->pFile->m_drawParam.topSpace)
		return;
	if(pDoc->m_Cursor==VZOOM)
	{
		int i = pDoc->pFile->getPointChan(GetScrollPosition().y,point.y);
		pDoc->pFile->AVector[i]->m_drawPara.VSpace = pDoc->pFile->AVector[i]->m_drawPara.defultVSpace;
		pDoc->pFile->AVector[i]->m_drawPara.UPScale = pDoc->pFile->AVector[i]->m_drawPara.defaultUPScale;
	}
	Invalidate();
	UpdateWindow();
	CScrollView::OnRButtonDblClk(nFlags, point);
}


//BOOL CAView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	HCURSOR hCur  =  LoadCursor( NULL  , MAKEINTRESOURCE(105)) ;
//                     ::SetCursor(hCur);
//	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
//}


int CAView::drawAxis(CDC* pDC, CPoint pPos, int startSample, int endSample, CComtrade* pFile)
{
	int i,x;
	int zero;
	double SPScale=pFile->m_drawParam.SPScale;
	double Rate0,Rate;
	double pxLen,t_pxLen,pxLen1,Len2Z;
	double div1/*,div2,div3*/;
	CRect bkgRect;
	this->GetClientRect(&bkgRect);
	int width0=bkgRect.Width()-pFile->m_drawParam.rightSpace;
	int width=bkgRect.Width()-pFile->m_drawParam.rightSpace-pFile->m_drawParam.leftSpace;
	CComTime lab;
	std::list<CPoint> Marks;
	std::list<CPoint>::iterator iMk0;
	std::list<CPoint>::iterator iMk1;
	CPoint pPoint=GetScrollPosition();
	CPen* pOld;
	/*CPen APenG1;
	CPen APenG2;
	CPen APenG3;*/

	//APenG1.CreatePen(PS_SOLID,1,Gray00);
	//APenG2.CreatePen(PS_SOLID,1,Gray0);
	//APenG3.CreatePen(PS_SOLID,1,Gray1);
	extern CFA2013App theApp;

	pOld = pDC->SelectObject(&theApp.APenG1);
	pDC->MoveTo(pPos);
	pDC->LineTo(pPos.x+width,pPos.y);
	pDC->SelectObject(&theApp.APenG2);
	pDC->MoveTo(pPos.x,pPos.y+1);
	pDC->LineTo(pPos.x+width,pPos.y+1);
	pDC->SelectObject(&theApp.APenG3);
	pDC->MoveTo(pPos.x,pPos.y+2);
	pDC->LineTo(pPos.x+width,pPos.y+2);

	for(i=0;i<pFile->m_TotalSampleNum;++i)
	{
		if(pFile->Time[i]==0)
			zero = i;
	}

	Rate0=getSampleRate(zero,pFile->m_SampleRate);
	for(i=1;i<=100;++i)
	{
		pxLen=i*0.01*Rate0*SPScale;
		t_pxLen=(i+1)*0.01*Rate0*SPScale;
		if(fabs(pxLen-150)<fabs(t_pxLen-150))
		{
			div1=i*0.01;
			pxLen1=pxLen/10;
			break;
		}
	}
	pxLen1=pxLen/10;
	//////测试用输出语句//////////////////////////////
	CString a;
	//a.Format(_T("%f，%f,%f"),div1,pxLen,t_pxLen);
	//pDC->TextOutW(800,0,a);
	///////////////////////////////////////////////
	Len2Z=zero*SPScale;
	if(pPoint.x<=Len2Z)
	{
		Marks.push_front(CPoint((int)((zero-startSample)*SPScale+pPos.x),0));
		for(i=1;i*pxLen<Len2Z;i++)
		{
			x=(int)((zero-startSample)*SPScale+pPos.x-i*pxLen);
			if (x<width0&&x>=pPos.x)
				Marks.push_front(CPoint(x,1));
		}
	}
	for(i=0;(zero-startSample)*SPScale+pPos.x+i*pxLen<=width0;++i)
	{
		x=(int)((zero-startSample)*SPScale+pPos.x+i*pxLen);
		if (x>=pPos.x)
			Marks.push_back(CPoint(x,1));
	}
	////////////生成小刻度//////////////////////////////////////////
	iMk0=Marks.begin();
	iMk1=Marks.end();

	for(;iMk0!=iMk1;++iMk0)
	{
		for(i=9;i>0;--i)
		{
			x=(int)((*iMk0).x-i*pxLen1);
			if(x<width0&&x>=pPos.x)
				Marks.insert(iMk0,CPoint(x,2));
		}
	}
	--iMk0;
	for(i=1;i<10;++i)
	{
		x=(int)((*iMk0).x+i*pxLen1);
			if(x<width0&&x>=pPos.x)
		Marks.insert(iMk1,CPoint((int)((*iMk0).x+i*pxLen1),2));
	}
	///////////////////////////////////////////////////////

	iMk0=Marks.begin();
	iMk1=Marks.end();
	for(;iMk0!=iMk1;++iMk0)
	{
		x=(*iMk0).y;
		if (x==0)
		{
			drawMark(pDC,CPoint((*iMk0).x,pPos.y),4,6);
			drawMark(pDC,CPoint((*iMk0).x-1,pPos.y),4,1);
			drawMark(pDC,CPoint((*iMk0).x+1,pPos.y),4,1);
		}
		else if (x==1)
		{
			double px2s = pPoint.x+(*iMk0).x-pPos.x;
			px2s/=SPScale;
			if(px2s<pFile->m_TotalSampleNum)
			{
			lab=pFile->m_StartTime + pFile->Time[(unsigned int)(px2s-1)];
			a=lab.PrintTime();
			CFont* def_font = pDC->SelectObject(&theApp.font);
			pDC->SetTextColor(White);
			pDC->SetBkMode(TRANSPARENT);
			pDC->TextOutW((*iMk0).x-50,40,a);
			pDC->SelectObject(def_font);
			}
			drawMark(pDC,CPoint((*iMk0).x,pPos.y),5,6);
		}
		else if (x==2)
			drawMark(pDC,CPoint((*iMk0).x,pPos.y),0,4);
	}

	for(i=startSample;i<endSample;++i)
	{
		Rate=getSampleRate(i,pFile->m_SampleRate);
	}

	pDC->SelectObject(pOld);
	return 0;
}

int CAView::drawMark(CDC* pDC,CPoint pPos, int up, int down)
{
	CPen *APen,*pOld;
	extern CFA2013App theApp;
	if(up>2)
		APen = &theApp.PenN;
	else
		APen = &theApp.APenG2;
	pOld = pDC->SelectObject(APen);
	pDC->MoveTo(pPos.x,pPos.y+down);
	pDC->LineTo(pPos.x,pPos.y-up);

	pDC->SelectObject(pOld);
	return 0;
}


double CAView::getSampleRate(int SamplePoint,std::vector<SampleRate>& m_SampleRate)
{
	double SampRate = 0;
	if (SamplePoint >= m_SampleRate[0].samp*0.02 && SamplePoint <= m_SampleRate[0].endsamp)
		SampRate = m_SampleRate[0].samp;
	else if (SamplePoint < m_SampleRate[0].endsamp)
		return 0;
	else
	{
		for (int i = 1; i < (int)m_SampleRate.size(); ++i)   //size类型
		{
			if (SamplePoint <= m_SampleRate[i].endsamp && SamplePoint >= m_SampleRate[i-1].endsamp + m_SampleRate[i].samp*0.02)
			{
				SampRate = m_SampleRate[i].samp;
				break;
			}
		}
		if (SampRate == 0)
			return 0;
	}
	return SampRate;
}


void CAView::OnReset()
{
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->pFile==NULL)
		return;
	pDoc->pFile->m_drawParam.SPScale=pDoc->pFile->m_drawParam.defaultSPScale;
	pDoc->pFile->m_drawParam.LastSPScale=pDoc->pFile->m_drawParam.defaultSPScale;
	for(int i=0;i<pDoc->pFile->m_ANumber;++i)
	{
		pDoc->pFile->AVector[i]->m_drawPara.UPScale=pDoc->pFile->AVector[i]->m_drawPara.defaultUPScale;
		pDoc->pFile->AVector[i]->m_drawPara.VSpace=pDoc->pFile->AVector[i]->m_drawPara.defultVSpace;
	}
	Invalidate();
	UpdateWindow();
}


void CAView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	pDoc->UpdateAllViews(NULL);
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CAView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	pDoc->UpdateAllViews(NULL);
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CAView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	switch(nChar)
	{
	case VK_LEFT:
		if (pDoc->CursorNum1>0)
		{
			pDoc->CursorNum1--;
		}
		else
		{
			pDoc->CursorNum1=0;
		}
		break;
	case VK_RIGHT:
		if (pDoc->CursorNum1<pDoc->pFile->m_TotalSampleNum-1)
		{
			pDoc->CursorNum1++;
		}
		else
		{
			pDoc->CursorNum1=pDoc->pFile->m_TotalSampleNum-1;
		}
		break;
	case VK_UP:
		if (pDoc->ChanNum1>0)
		{
			--pDoc->ChanNum1;
		}
		break;
	case VK_DOWN:
		if (pDoc->ChanNum1<pDoc->pFile->m_ANumber-1)
		{
			++pDoc->ChanNum1;
		}
		break;
	}
	pDoc->pCursor1.x = (int)(pDoc->CursorNum1 * pDoc->pFile->m_drawParam.SPScale + pDoc->pFile->m_drawParam.leftSpace);
	pDoc->UpdateAllViews(NULL);
	Invalidate();
	UpdateWindow();
	if(pDoc->pDlgForm!=NULL)
	{
		if ( pDoc->pDlgForm->IsWindowVisible() == 1)
		{
			pDoc->pDlgForm->AList.SetItemState(pDoc->ChanNum1,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
			pDoc->pDlgForm->AList.EnsureVisible(pDoc->ChanNum1,FALSE);
			pDoc->pDlgForm->AList.SetSelectionMark(pDoc->ChanNum1);
			pDoc->pDlgForm->AList.SetFocus();
		}
	}
	if(pDoc->pDlgHarmo!=NULL)
	{
		if ( pDoc->pDlgHarmo->IsWindowVisible() == 1)
		{
			pDoc->pDlgHarmo->m_pView->Invalidate();
			pDoc->pDlgHarmo->m_pView->UpdateWindow();
			pDoc->pDlgHarmo->Alist.SetCurSel(pDoc->ChanNum1);
		}
	}
	if(pDoc->pDlgPhase!=NULL)
	{
		if ( pDoc->pDlgPhase->IsWindowVisible() == 1)
		{
			pDoc->pDlgPhase->m_pView->Invalidate();
			pDoc->pDlgPhase->m_pView->UpdateWindow();
		}
	}
	if(pDoc->pDlgSeqComp!=NULL)
	{
		if ( pDoc->pDlgSeqComp->IsWindowVisible() == 1)
		{
			pDoc->pDlgSeqComp->m_pPView->Invalidate();
			pDoc->pDlgSeqComp->m_pSView->Invalidate();
			pDoc->pDlgSeqComp->m_pPView->UpdateWindow();
			pDoc->pDlgSeqComp->m_pSView->UpdateWindow();
		}
	}
	if(pDoc->pDlgImp!=NULL)
	{
		if ( pDoc->pDlgImp->IsWindowVisible() == 1)
		{
			pDoc->pDlgImp->m_pView->Invalidate();
			pDoc->pDlgImp->m_pView->UpdateWindow();
		}
	}
	if(pDoc->pDlgBusDiff!=NULL)
	{
		if ( pDoc->pDlgBusDiff->IsWindowVisible() == 1)
		{
			pDoc->pDlgBusDiff->m_pView->Invalidate();
			pDoc->pDlgBusDiff->m_pView->UpdateWindow();
		}
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CAView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ScrollToPosition(CPoint(GetScrollPosition().x,GetScrollPosition().y-zDelta));
	CFA2013Doc* pDoc =(CFA2013Doc*)GetDocument();
	Invalidate();
	UpdateWindow();
	pDoc->UpdateAllViews(NULL);
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}
