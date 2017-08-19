// DlgBusDiff.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgBusDiff.h"
#include "afxdialogex.h"
#include<complex>
#include "DlgBusDiffData.h"
class CDlgBusDiffData;

// CDlgBusDiff 对话框

IMPLEMENT_DYNAMIC(CDlgBusDiff, CDialogEx)

CDlgBusDiff::CDlgBusDiff(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBusDiff::IDD, pParent)
, Ia1(-1)
, Ia2(-1)
, Ib1(-1)
, Ib2(-1)
, Ic1(-1)
, Ic2(-1)
, LocFlag(0)
{
	Im=8.0;
	Iop=0.3;
	Iopres=1.2;
	Kres=0.8;
	Iresm=(Im-Iop)/Kres+Iopres;
}

CDlgBusDiff::~CDlgBusDiff()
{
}

void CDlgBusDiff::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IA1LIST, Ia1List);
	DDX_Control(pDX, IDC_IB1LIST, Ib1List);
	DDX_Control(pDX, IDC_IC1LIST, Ic1List);
	DDX_Control(pDX, IDC_IA2LIST, Ia2List);
	DDX_Control(pDX, IDC_IB2LIST, Ib2List);
	DDX_Control(pDX, IDC_IC2LIST, Ic2List);
	DDX_Control(pDX, IDC_BusDiffType, BDTypeList);
}


BEGIN_MESSAGE_MAP(CDlgBusDiff, CDialogEx)
	ON_CBN_SELCHANGE(IDC_IA1LIST, &CDlgBusDiff::OnCbnSelchangeIa1list)
	ON_CBN_SELCHANGE(IDC_IB1LIST, &CDlgBusDiff::OnCbnSelchangeIb1list)
	ON_CBN_SELCHANGE(IDC_IC1LIST, &CDlgBusDiff::OnCbnSelchangeIc1list)
	ON_CBN_SELCHANGE(IDC_IA2LIST, &CDlgBusDiff::OnCbnSelchangeIa2list)
	ON_CBN_SELCHANGE(IDC_IB2LIST, &CDlgBusDiff::OnCbnSelchangeIb2list)
	ON_CBN_SELCHANGE(IDC_IC2LIST, &CDlgBusDiff::OnCbnSelchangeIc2list)
	ON_BN_CLICKED(IDC_LocVisable, &CDlgBusDiff::OnBnClickedLocvisable)
	ON_BN_CLICKED(IDC_diinc, &CDlgBusDiff::OnBnClickeddiinc)
	ON_BN_CLICKED(IDC_didec, &CDlgBusDiff::OnBnClickeddidec)
	ON_BN_CLICKED(IDC_dirst, &CDlgBusDiff::OnBnClickeddirst)
	ON_CBN_SELCHANGE(IDC_BusDiffType, &CDlgBusDiff::OnCbnSelchangeBusdifftype)
	ON_BN_CLICKED(IDC_SetData, &CDlgBusDiff::OnBnClickedSetdata)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgBusDiff 消息处理程序
// C:\Users\炎侃\Desktop\FA20130428\FA2013\DlgBusDiff.cpp : 实现文件
//




// CBusDiffView

IMPLEMENT_DYNCREATE(CBusDiffView, CView)

CBusDiffView::CBusDiffView()
: MidP(0,0)
{
	IPS = DefaultIPS = 0.05;
	SPD = DefaultSPD = 20;
}

CBusDiffView::~CBusDiffView()
{
}

BEGIN_MESSAGE_MAP(CBusDiffView, CView)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CBusDiffView 绘图

void CBusDiffView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
	DrawBusDiff(pDC);
}


// CBusDiffView 诊断

#ifdef _DEBUG
void CBusDiffView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBusDiffView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBusDiffView 消息处理程序


BOOL CDlgBusDiff::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UINT TargetCtrlID = IDC_BDVIEW;  
	CWnd *pWnd = this->GetDlgItem(TargetCtrlID);  
	CRect RectTargetCtrl;
	pWnd->GetWindowRect(RectTargetCtrl);  
	this->ScreenToClient(RectTargetCtrl);  
	m_pView = (CBusDiffView*)RUNTIME_CLASS(CBusDiffView)->CreateObject();
	(m_pView->pDoc)=pDoc;
	//在目标位置动态创建视图
	if (NULL==m_pView)  
	{  
	return FALSE;  
	}  
	m_pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, RectTargetCtrl, this, TargetCtrlID);  
	m_pView->m_pDlg = this;
	//列表
	int iA1,iB1,iC1,iA2,iB2,iC2;
	iA1=iB1=iC1=iA2=iB2=iC2=0;
	for(int i=0; i<pDoc->pFile->m_ANumber;i++)
	{
		CString tempCS;
		tempCS.Format(_T("%d:"),i+1);
		tempCS += pDoc->pFile->AVector[i]->name;
		CString tempUU = pDoc->pFile->AVector[i]->uu;
        if ((tempUU == _T("kA"))||(tempUU == _T("A")))
		{
			if ((pDoc->pFile->AVector[i]->ph == _T("A"))||(pDoc->pFile->AVector[i]->ph == _T("a")))
			{
				Ia1List.InsertString(iA1++,tempCS);
				Ia2List.InsertString(iA2++,tempCS);
			}
			else if ((pDoc->pFile->AVector[i]->ph == _T("B"))||(pDoc->pFile->AVector[i]->ph == _T("b")))
			{
				Ib1List.InsertString(iB1++,tempCS);
				Ib2List.InsertString(iB2++,tempCS);
			}
			else if ((pDoc->pFile->AVector[i]->ph == _T("C"))||(pDoc->pFile->AVector[i]->ph == _T("c")))
			{
				Ic1List.InsertString(iC1++,tempCS);
				Ic2List.InsertString(iC2++,tempCS);
			}
		}
	}
	Ia1List.SetCurSel(1);
	Ib1List.SetCurSel(1);
	Ic1List.SetCurSel(1);
	Ia2List.SetCurSel(1);
	Ib2List.SetCurSel(1);
	Ic2List.SetCurSel(1);
	BDTypeList.InsertString(0,_T("和差制动"));
	BDTypeList.InsertString(1,_T("比率制动"));
	BDTypeList.InsertString(2,_T("标积制动"));
	BDTypeList.SetCurSel(0);
	//endof列表
	CDlgBusDiff::OnCbnSelchangeIc1list();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgBusDiff::OnCbnSelchangeIa1list()
{
	// TODO: 在此添加控件通知处理程序代码
	//自动填充
	int a,curPos;
	CString temp,tempA,tempBC;
	Ia1List.GetLBText(Ia1List.GetCurSel(),temp);
	curPos=0;
	a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
	tempA = pDoc->pFile->AVector[a-1]->ccbm;
	int numB = Ib1List.GetCount();
	int numC = Ic1List.GetCount();
	int curB=0;
	int curC=0;
	while(curB<numB)
	{
		Ib1List.SetCurSel(curB);
		Ib1List.GetLBText(Ib1List.GetCurSel(),temp);
		curPos=0;
	    a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
		tempBC = pDoc->pFile->AVector[a-1]->ccbm;
		if (tempBC==tempA)
			break;
		else
			curB++;
	}
	while(curC<numC)
	{
		Ic1List.SetCurSel(curC);
		Ic1List.GetLBText(Ic1List.GetCurSel(),temp);
		curPos=0;
	    a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
		tempBC = pDoc->pFile->AVector[a-1]->ccbm;
		if (tempBC==tempA)
			break;
		else
			curC++;
	}
	//自动填充完毕
	CDlgBusDiff::OnCbnSelchangeIc1list();
}


void CDlgBusDiff::OnCbnSelchangeIb1list()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgBusDiff::OnCbnSelchangeIc1list();
}


void CDlgBusDiff::OnCbnSelchangeIc1list()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tempIA1,tempIB1,tempIC1,tempIA2,tempIB2,tempIC2,tempI1,tempI2,tempI1CS,tempI2CS;
	int IA1,IB1,IC1,IA2,IB2,IC2,curPos;
	Ia1List.GetLBText(Ia1List.GetCurSel(),tempIA1);
	Ib1List.GetLBText(Ib1List.GetCurSel(),tempIB1);
	Ic1List.GetLBText(Ic1List.GetCurSel(),tempIC1);
	Ia2List.GetLBText(Ia2List.GetCurSel(),tempIA2);
	Ib2List.GetLBText(Ib2List.GetCurSel(),tempIB2);
	Ic2List.GetLBText(Ic2List.GetCurSel(),tempIC2);
	curPos = 0;
	Ia1 = IA1 = _wtoi((tempIA1.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	Ib1 = IB1 = _wtoi((tempIB1.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	Ic1 = IC1 = _wtoi((tempIC1.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	Ia2 = IA2 = _wtoi((tempIA2.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	Ib2 = IB2 = _wtoi((tempIB2.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	Ic2 = IC2 = _wtoi((tempIC2.Tokenize(_T(":"), curPos).GetBuffer()));
	tempI1 = pDoc->pFile->AVector[IA1-1]->ccbm;
	tempI2 = pDoc->pFile->AVector[IA2-1]->ccbm;
	CWnd* pWnd = GetDlgItem(IDC_BusSelStatus);
	if ((pDoc->pFile->AVector[IB1-1]->ccbm == tempI1)&&(pDoc->pFile->AVector[IC1-1]->ccbm == tempI1))
		tempI1CS = _T("电流通道1:")+tempI1;
	else
		tempI1CS = _T("电流通道2:无意义的组合。");
	if ((pDoc->pFile->AVector[IB2-1]->ccbm == tempI2)&&(pDoc->pFile->AVector[IC2-1]->ccbm == tempI2))
		tempI2CS = _T("电流通道2:")+tempI2;
	else
		tempI2CS = _T("电流通道2:无意义的组合。");
	pWnd->SetWindowTextW(tempI1CS+ _T("\n") +tempI2CS +_T("\n请选择相同元件的A、B、C三相通道。"));
	this->m_pView->Invalidate();
}


void CDlgBusDiff::OnCbnSelchangeIa2list()
{
	// TODO: 在此添加控件通知处理程序代码
	//自动填充
	int a,curPos;
	CString temp,tempA,tempBC;
	Ia2List.GetLBText(Ia2List.GetCurSel(),temp);
	curPos=0;
	a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
	tempA = pDoc->pFile->AVector[a-1]->ccbm;
	int numB = Ib2List.GetCount();
	int numC = Ic2List.GetCount();
	int curB=0;
	int curC=0;
	while(curB<numB)
	{
		Ib2List.SetCurSel(curB);
		Ib2List.GetLBText(Ib2List.GetCurSel(),temp);
		curPos=0;
	    a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
		tempBC = pDoc->pFile->AVector[a-1]->ccbm;
		if (tempBC==tempA)
			break;
		else
			curB++;
	}
	while(curC<numC)
	{
		Ic2List.SetCurSel(curC);
		Ic2List.GetLBText(Ic2List.GetCurSel(),temp);
		curPos=0;
	    a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
		tempBC = pDoc->pFile->AVector[a-1]->ccbm;
		if (tempBC==tempA)
			break;
		else
			curC++;
	}
	//自动填充完毕
	CDlgBusDiff::OnCbnSelchangeIc1list();
}


void CDlgBusDiff::OnCbnSelchangeIb2list()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgBusDiff::OnCbnSelchangeIc1list();
}


void CDlgBusDiff::OnCbnSelchangeIc2list()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgBusDiff::OnCbnSelchangeIc1list();
}


int CBusDiffView::DrawBusDiff(CDC* pDC)
{
	CRect rect;
	extern CFA2013App theApp;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect,RGB(0,0,0));//背景
	
	//坐标+网格
	int space = rect.Width()*0.05;
	MidP.x = space;
	MidP.y = rect.bottom-space;
	//动作区
	pDC->SelectObject(&theApp.BrushBk);
	pDC->BeginPath();
	pDC->MoveTo(MidP.x,MidP.y-m_pDlg->Iop/IPS);
	pDC->LineTo(MidP.x+m_pDlg->Iopres/IPS,MidP.y-m_pDlg->Iop/IPS);
	pDC->LineTo(MidP.x+m_pDlg->Iresm/IPS,MidP.y-m_pDlg->Im/IPS);
	pDC->LineTo(MidP.x,MidP.y-m_pDlg->Im/IPS);
	pDC->LineTo(MidP.x,MidP.y-m_pDlg->Iop/IPS);
	pDC->EndPath();
	pDC->FillPath();
	//
	CPen* pOld = pDC->SelectObject(&theApp.PenGridGray);
	
	int nXa,nYl,nXb,nYr;
	nXa=nYl=nXb=nYr=1;
	CString TempCS;
	CFont* def_font = pDC->SelectObject(&theApp.font);
	pDC->SetTextColor(White);
	TempCS.Format(_T("网格刻度: %.3f A/格"),SPD*IPS);
	pDC->TextOutW(rect.left+space,rect.bottom-20,TempCS);
	while(MidP.x + SPD*nYr < rect.right-space)
	{
		pDC->MoveTo(MidP.x + SPD*nYr,rect.top+space);
		pDC->LineTo(MidP.x + SPD*nYr,rect.bottom-space);
		nYr++;
	}
	while(MidP.x - SPD*nYl > rect.left+space)
	{
		pDC->MoveTo(MidP.x - SPD*nYl,rect.top+space);
		pDC->LineTo(MidP.x - SPD*nYl,rect.bottom-space);
		nYl++;
	}
	while(MidP.y + SPD*nXb < rect.bottom-space)
	{
		pDC->MoveTo(rect.left+space,MidP.y + SPD*nXb);
		pDC->LineTo(rect.right-space,MidP.y + SPD*nXb);
		nXb++;
	}
	while(MidP.y - SPD*nXa > rect.top+space)
	{
		pDC->MoveTo(rect.left+space,MidP.y - SPD*nXa);
		pDC->LineTo(rect.right-space,MidP.y - SPD*nXa);
		nXa++;
	}
	pOld = pDC->SelectObject(&theApp.PenN);
	//x轴
	pDC->MoveTo(MidP);
	pDC->LineTo(rect.right-space,MidP.y);
	pDC->TextOutW(rect.right-100,rect.bottom-20,_T("制动电流"));
	//y轴
	pDC->MoveTo(MidP);
	pDC->LineTo(MidP.x,space);
	pDC->TextOutW(MidP.x,0,_T("差动电流"));
	pDC->SelectObject(pOld);
	CBrush* pOldb = pDC->SelectObject(&theApp.BrushN);
	//X箭头
	pDC->BeginPath();
	pDC->MoveTo(rect.right-space,MidP.y);
	pDC->LineTo(rect.right-space-10,MidP.y+7);
	pDC->LineTo(rect.right-space-10,MidP.y-7);
	pDC->LineTo(rect.right-space,MidP.y);
	pDC->EndPath();
	pDC->FillPath();
	//Y箭头
	pDC->BeginPath();
	pDC->MoveTo(MidP.x,space);
	pDC->LineTo(MidP.x-7,space+10);
	pDC->LineTo(MidP.x+7,space+10);
	pDC->LineTo(MidP.x,space);
	pDC->EndPath();
	pDC->FillPath();
	//母差分析
	int Type =m_pDlg->BDTypeList.GetCurSel()+1;
	CursorNum1 = pDoc->CursorNum1;
	if (0==m_pDlg->LocFlag)//点
	{
		int Flag=1;
		Flag=pDoc->pFile->GetCurrentBusDiff(CursorNum1+1,m_pDlg->Ia1,m_pDlg->Ib1,m_pDlg->Ic1,m_pDlg->Ia2,m_pDlg->Ib2,m_pDlg->Ic2,Type);
		if (Flag==1)
		{
			pDC->TextOutW(3*space,3*space,_T("·该区域无法计算，请将光标后移！"));
			return 0;
		}
		pOld = pDC->SelectObject(&theApp.PenAD);
		pDC->MoveTo(MidP);
		pDC->LineTo(MidP.x + pDoc->pFile->dI.resIA/IPS,MidP.y - pDoc->pFile->dI.rIA/IPS);
		pDC->Ellipse(MidP.x + pDoc->pFile->dI.resIA/IPS-3,MidP.y - pDoc->pFile->dI.rIA/IPS-3,MidP.x + pDoc->pFile->dI.resIA/IPS+3,MidP.y - pDoc->pFile->dI.rIA/IPS+3);
		pOld = pDC->SelectObject(&theApp.PenBD);
		pDC->MoveTo(MidP);
		pDC->LineTo(MidP.x + pDoc->pFile->dI.resIB/IPS,MidP.y - pDoc->pFile->dI.rIB/IPS);
		pDC->Ellipse(MidP.x + pDoc->pFile->dI.resIB/IPS-3,MidP.y - pDoc->pFile->dI.rIB/IPS-3,MidP.x + pDoc->pFile->dI.resIB/IPS+3,MidP.y - pDoc->pFile->dI.rIB/IPS+3);
		pOld = pDC->SelectObject(&theApp.PenCD);
		pDC->MoveTo(MidP);
		pDC->LineTo(MidP.x + pDoc->pFile->dI.resIC/IPS,MidP.y - pDoc->pFile->dI.rIC/IPS);
		pDC->Ellipse(MidP.x + pDoc->pFile->dI.resIC/IPS-3,MidP.y - pDoc->pFile->dI.rIC/IPS-3,MidP.x + pDoc->pFile->dI.resIC/IPS+3,MidP.y - pDoc->pFile->dI.rIC/IPS+3);
	}
	else
	{
		CPoint PnowA,PlastA,PnowB,PlastB,PnowC,PlastC;
		int Pcount=0;
		int c_Flag;
		for(int i=0; i<=CursorNum1; i++)
		{
			c_Flag =pDoc->pFile->GetCurrentBusDiff(i+1,m_pDlg->Ia1,m_pDlg->Ib1,m_pDlg->Ic1,m_pDlg->Ia2,m_pDlg->Ib2,m_pDlg->Ic2,Type);
			if (c_Flag==1)
			{
				Pcount = 0;
				continue;
			}
			else
			{
				Pcount++;
				//A相
				PnowA.x = MidP.x + (pDoc->pFile->dI.resIA)/IPS;
				PnowA.y = MidP.y - (pDoc->pFile->dI.rIA)/IPS;
				PnowB.x = MidP.x + (pDoc->pFile->dI.resIB)/IPS;
				PnowB.y = MidP.y - (pDoc->pFile->dI.rIB)/IPS;
				PnowC.x = MidP.x + (pDoc->pFile->dI.resIC)/IPS;
				PnowC.y = MidP.y - (pDoc->pFile->dI.rIC)/IPS;
				pOld = pDC->SelectObject(&theApp.PenA);
				if (((PnowA.x<rect.right)&&(PnowA.x>rect.left)&&(PnowA.y<rect.bottom)&&(PnowA.y>rect.top))
					||((PlastA.x<rect.right)&&(PlastA.x>rect.left)&&(PlastA.y<rect.bottom)&&(PlastA.y>rect.top)))
				{
					if (Pcount==1)
						pDC->MoveTo(PnowA);
					else
					{
						pDC->MoveTo(PlastA);
						pDC->LineTo(PnowA);
					}
				}
				//B相
				pOld = pDC->SelectObject(&theApp.PenB);
				if (((PnowB.x<rect.right)&&(PnowB.x>rect.left)&&(PnowB.y<rect.bottom)&&(PnowB.y>rect.top))
					||((PlastB.x<rect.right)&&(PlastB.x>rect.left)&&(PlastB.y<rect.bottom)&&(PlastB.y>rect.top)))
				{
					if (Pcount==1)
						pDC->MoveTo(PnowB);
					else
					{
						pDC->MoveTo(PlastB);
						pDC->LineTo(PnowB);
					}
				}
				//C相
				pOld = pDC->SelectObject(&theApp.PenC);
				if (((PnowC.x<rect.right)&&(PnowC.x>rect.left)&&(PnowC.y<rect.bottom)&&(PnowC.y>rect.top))
					||((PlastC.x<rect.right)&&(PlastC.x>rect.left)&&(PlastC.y<rect.bottom)&&(PlastC.y>rect.top)))
				{
					if (Pcount==1)
						pDC->MoveTo(PnowC);
					else
					{
						pDC->MoveTo(PlastC);
						pDC->LineTo(PnowC);
					}
				}
				PlastA = PnowA; PlastB = PnowB; PlastC = PnowC;
			
			}
		}
		if (c_Flag!=1)
		{
			pOld = pDC->SelectObject(&theApp.PenAD);
			pDC->MoveTo(MidP);
			pDC->LineTo(MidP.x + pDoc->pFile->dI.resIA/IPS,MidP.y - pDoc->pFile->dI.rIA/IPS);
			pDC->Ellipse(MidP.x + pDoc->pFile->dI.resIA/IPS-3,MidP.y - pDoc->pFile->dI.rIA/IPS-3,MidP.x + pDoc->pFile->dI.resIA/IPS+3,MidP.y - pDoc->pFile->dI.rIA/IPS+3);
			pOld = pDC->SelectObject(&theApp.PenBD);
			pDC->MoveTo(MidP);
			pDC->LineTo(MidP.x + pDoc->pFile->dI.resIB/IPS,MidP.y - pDoc->pFile->dI.rIB/IPS);
			pDC->Ellipse(MidP.x + pDoc->pFile->dI.resIB/IPS-3,MidP.y - pDoc->pFile->dI.rIB/IPS-3,MidP.x + pDoc->pFile->dI.resIB/IPS+3,MidP.y - pDoc->pFile->dI.rIB/IPS+3);
			pOld = pDC->SelectObject(&theApp.PenCD);
			pDC->MoveTo(MidP);
			pDC->LineTo(MidP.x + pDoc->pFile->dI.resIC/IPS,MidP.y - pDoc->pFile->dI.rIC/IPS);
			pDC->Ellipse(MidP.x + pDoc->pFile->dI.resIC/IPS-3,MidP.y - pDoc->pFile->dI.rIC/IPS-3,MidP.x + pDoc->pFile->dI.resIC/IPS+3,MidP.y - pDoc->pFile->dI.rIC/IPS+3);
		}
	}
	return 0;
}


void CDlgBusDiff::OnBnClickedLocvisable()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pWnd = GetDlgItem(IDC_LocVisable);
	pWnd->EnableWindow(FALSE);
	if (LocFlag==0)
	{
		LocFlag=1;
		pWnd->SetWindowTextW(_T("隐藏轨迹"));
	}
	else if (LocFlag==1)
	{
		LocFlag=0;
		pWnd->SetWindowTextW(_T("显示轨迹"));
	}
	this->m_pView->Invalidate();
	pWnd->EnableWindow(TRUE);
}


void CDlgBusDiff::OnBnClickeddiinc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pView->IPS/2 >0.005)
		m_pView->IPS = m_pView->IPS / 2;
	this->m_pView->Invalidate();
}


void CDlgBusDiff::OnBnClickeddidec()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pView->IPS*2 <1)
		m_pView->IPS = m_pView->IPS * 2;
	this->m_pView->Invalidate();
}


void CDlgBusDiff::OnBnClickeddirst()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_pView->IPS = this->m_pView->DefaultIPS;
	this->m_pView->SPD = this->m_pView->DefaultSPD;
	this->m_pView->Invalidate();
}


int CBusDiffView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	//return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CDlgBusDiff::OnCbnSelchangeBusdifftype()
{
	// TODO: 在此添加控件通知处理程序代码
	this->m_pView->Invalidate();
}


void CDlgBusDiff::OnBnClickedSetdata()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgBusDiffData pDlg;
	if(pDlg.DoModal() == IDOK)
	{
		Im=pDlg.Im;
		Iop=pDlg.Iop;
		Iopres=pDlg.Iopres;
		Kres=pDlg.Kres;
		Iresm=(Im-Iop)/Kres+Iopres;
	}
	this->m_pView->Invalidate();
}


void CDlgBusDiff::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	DestroyWindow();
	//CDialogEx::OnCancel();
}


void CDlgBusDiff::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->pDoc->pDlgBusDiff = NULL;
	delete this;
	CDialogEx::PostNcDestroy();
}


void CDlgBusDiff::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	//CDialogEx::OnClose();
}
