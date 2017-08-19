// DlgSeqComp.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgSeqComp.h"
#include "afxdialogex.h"
#include"splab_vector.h"

// CDlgSeqComp 对话框

IMPLEMENT_DYNAMIC(CDlgSeqComp, CDialog)

CDlgSeqComp::CDlgSeqComp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSeqComp::IDD, pParent)
	, Pps(1.0)
	, Sps(1.0)
	, m_Mode(0)
{

}

CDlgSeqComp::~CDlgSeqComp()
{
}

void CDlgSeqComp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ChanA, ChanA);
	DDX_Control(pDX, IDC_ChanB, ChanB);
	DDX_Control(pDX, IDC_ChanC, ChanC);
	DDX_Control(pDX, IDC_PhaseDataList, PDList);
	DDX_Control(pDX, IDC_SeqCompList, SDList);
}


BEGIN_MESSAGE_MAP(CDlgSeqComp, CDialog)
	ON_CBN_SELCHANGE(IDC_ChanA, &CDlgSeqComp::OnCbnSelchangeChana)
	ON_CBN_SELCHANGE(IDC_ChanB, &CDlgSeqComp::OnCbnSelchangeChanb)
	ON_CBN_SELCHANGE(IDC_ChanC, &CDlgSeqComp::OnCbnSelchangeChanc)
	ON_BN_CLICKED(IDC_Pinc, &CDlgSeqComp::OnBnClickedPinc)
	ON_BN_CLICKED(IDC_Pdec, &CDlgSeqComp::OnBnClickedPdec)
	ON_BN_CLICKED(IDC_Sinc, &CDlgSeqComp::OnBnClickedSinc)
	ON_BN_CLICKED(IDC_Sdec, &CDlgSeqComp::OnBnClickedSdec)
	ON_BN_CLICKED(IDC_Pini, &CDlgSeqComp::OnBnClickedPini)
	ON_BN_CLICKED(IDC_Sini, &CDlgSeqComp::OnBnClickedSini)
	ON_BN_CLICKED(IDC_TurnMode, &CDlgSeqComp::OnBnClickedTurnmode)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgSeqComp 消息处理程序


BOOL CDlgSeqComp::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int iA,iB,iC;
	iA=iB=iC=0;
	for(int i=0; i<pDoc->pFile->m_ANumber;i++)
	{
		CString tempCS;
		tempCS.Format(_T("%d:"),i+1);
		tempCS += pDoc->pFile->AVector[i]->name;
		if ((pDoc->pFile->AVector[i]->ph == _T("A"))||(pDoc->pFile->AVector[i]->ph == _T("a")))
			ChanA.InsertString(iA++,tempCS);
		else if ((pDoc->pFile->AVector[i]->ph == _T("B"))||(pDoc->pFile->AVector[i]->ph == _T("b")))
			ChanB.InsertString(iB++,tempCS);
		else if ((pDoc->pFile->AVector[i]->ph == _T("C"))||(pDoc->pFile->AVector[i]->ph == _T("c")))
			ChanC.InsertString(iC++,tempCS);
	}
	ChanA.SetCurSel(0);
	ChanB.SetCurSel(0);
	ChanC.SetCurSel(0);
	CString temp = pDoc->pFile->AVector[0]->ccbm;
	if ((pDoc->pFile->AVector[1]->ccbm == temp)&&(pDoc->pFile->AVector[1]->ccbm == temp))
	{
		CWnd* pWnd = GetDlgItem(IDC_ChanStatus);
		pWnd->SetWindowTextW(_T("已选择：\n")+temp);
	}
	//List
	PDList.SetExtendedStyle(LVS_EX_FLATSB
		|LVS_EX_FULLROWSELECT
		|LVS_EX_HEADERDRAGDROP
		|LVS_EX_ONECLICKACTIVATE
		|LVS_EX_GRIDLINES);
	PDList.InsertColumn(0,_T("相别"),2,100,0);
	PDList.InsertColumn(1,_T("幅值"),2,100,1);
	PDList.InsertColumn(2,_T("相位"),2,100,2);
	SDList.SetExtendedStyle(LVS_EX_FLATSB
		|LVS_EX_FULLROWSELECT
		|LVS_EX_HEADERDRAGDROP
		|LVS_EX_ONECLICKACTIVATE
		|LVS_EX_GRIDLINES);
	SDList.InsertColumn(0,_T("序别"),2,100,0);
	SDList.InsertColumn(1,_T("幅值"),2,100,1);
	SDList.InsertColumn(2,_T("相位"),2,100,2);
	UpdateData(TRUE);

	//P视图初始化
	UINT TargetCtrlID = IDC_PView;  
	CWnd *pWnd = this->GetDlgItem(TargetCtrlID);  
	CRect RectTargetCtrl;  
	pWnd->GetWindowRect(RectTargetCtrl);  
	this->ScreenToClient(RectTargetCtrl);  
	m_pPView = (CSeqPView*)RUNTIME_CLASS(CSeqPView)->CreateObject();
	(m_pPView->pDoc)=pDoc;
	//在目标位置动态创建视图
	if (NULL==m_pPView)  
	{  
	return FALSE;  
	}  
	m_pPView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, RectTargetCtrl, this, TargetCtrlID);  
	m_pPView->m_pDlg = this;
	//S视图初始化
	TargetCtrlID = IDC_SView;  
	pWnd = this->GetDlgItem(TargetCtrlID);   
	pWnd->GetWindowRect(RectTargetCtrl);  
	this->ScreenToClient(RectTargetCtrl);  
	m_pSView = (CSeqSView*)RUNTIME_CLASS(CSeqSView)->CreateObject();
	(m_pSView->pDoc)=pDoc;
	//在目标位置动态创建视图
	if (NULL==m_pSView)  
	{  
	return FALSE;  
	}  
	m_pSView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, RectTargetCtrl, this, TargetCtrlID);  
	m_pSView->m_pDlg = this;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSeqComp::OnCbnSelchangeChana()
{
	// TODO: 在此添加控件通知处理程序代码
	//自动填充
	int a,curPos;
	CString temp,tempccbm,tempuu,tempc,tempu;
	ChanA.GetLBText(ChanA.GetCurSel(),temp);
	curPos=0;
	a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
	tempccbm = pDoc->pFile->AVector[a-1]->ccbm;
	tempuu = pDoc->pFile->AVector[a-1]->uu;
	int numB = ChanB.GetCount();
	int numC = ChanC.GetCount();
	int curB=0;
	int curC=0;
	while(curB<numB)
	{
		ChanB.SetCurSel(curB);
		ChanB.GetLBText(ChanB.GetCurSel(),temp);
		curPos=0;
	    a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
		tempc = pDoc->pFile->AVector[a-1]->ccbm;
		tempu = pDoc->pFile->AVector[a-1]->uu;
		if ((tempccbm==tempc)&&(tempuu==tempu))
			break;
		else
			curB++;
	}
	while(curC<numC)
	{
		ChanC.SetCurSel(curC);
		ChanC.GetLBText(ChanC.GetCurSel(),temp);
		curPos=0;
	    a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
		tempc = pDoc->pFile->AVector[a-1]->ccbm;
		tempu = pDoc->pFile->AVector[a-1]->uu;
		if ((tempccbm==tempc)&&(tempuu==tempu))
			break;
		else
			curC++;
	}
	//自动填充完毕
	CDlgSeqComp::OnCbnSelchangeChanc();
}


void CDlgSeqComp::OnCbnSelchangeChanb()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgSeqComp::OnCbnSelchangeChanc();
}


void CDlgSeqComp::OnCbnSelchangeChanc()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tempA,tempB,tempC,temp;
	int A,B,C,curPos;
	ChanA.GetLBText(ChanA.GetCurSel(),tempA);
	ChanB.GetLBText(ChanB.GetCurSel(),tempB);
	ChanC.GetLBText(ChanC.GetCurSel(),tempC);
	
	curPos = 0;
	A = _wtoi((tempA.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	B = _wtoi((tempB.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	C = _wtoi((tempC.Tokenize(_T(":"), curPos).GetBuffer()));
	temp = pDoc->pFile->AVector[A-1]->ccbm;
	CWnd* pWnd = GetDlgItem(IDC_ChanStatus);
	if ((pDoc->pFile->AVector[B-1]->ccbm == temp)&&(pDoc->pFile->AVector[C-1]->ccbm == temp))
	{
		pWnd->SetWindowTextW(_T("已选择：\n")+temp);
		pDoc->view_Scroll(A-1);
		pDoc->UpdateAllViews(NULL);
	}
	else pWnd->SetWindowTextW(_T("无意义的通道组合。\n请选择相同元件的A、B、C三相通道。"));
	
	m_pPView->Invalidate();
	m_pSView->Invalidate();
}
// C:\Users\炎侃\Desktop\FA2013F\FA2013\DlgSeqComp.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgSeqComp.h"


// CSeqPView

IMPLEMENT_DYNCREATE(CSeqPView, CView)

CSeqPView::CSeqPView()
   :CursorNum1(0)
{

}

CSeqPView::~CSeqPView()
{
}

BEGIN_MESSAGE_MAP(CSeqPView, CView)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CSeqPView 绘图

void CSeqPView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
	DrawP(pDC);
}


// CSeqPView 诊断

#ifdef _DEBUG
void CSeqPView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSeqPView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSeqPView 消息处理程序
// C:\Users\炎侃\Desktop\FA2013F\FA2013\DlgSeqComp.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgSeqComp.h"


// CSeqSView

IMPLEMENT_DYNCREATE(CSeqSView, CView)

CSeqSView::CSeqSView()
   :CursorNum1(0)
{

}

CSeqSView::~CSeqSView()
{
}

BEGIN_MESSAGE_MAP(CSeqSView, CView)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CSeqSView 绘图

void CSeqSView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
	DrawS(pDC);
}


// CSeqSView 诊断

#ifdef _DEBUG
void CSeqSView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSeqSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSeqSView 消息处理程序


int CSeqPView::DrawP(CDC* pDC)
{
	m_pDlg->PDList.DeleteAllItems();
	//背景及坐标
	CRect rect;
	using splab::PI;
	extern CFA2013App theApp;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect,RGB(0,0,0));//背景
	//pDC->SetBkColor(RGB(0,0,0));
	CFont* def_font = pDC->SelectObject(&theApp.font);
	pDC->SetTextColor(White);
	pDC->SetBkMode(TRANSPARENT);
	int Vsp = (int)(rect.Height()*0.05);
	int R = (int)(rect.Height()*0.45);
	int Hsp = rect.Width()/2 - R;
	pDC->TextOutW(10,rect.Height()-20,_T("相量图"));
	CPoint center(rect.Width()/2,rect.Height()/2);
	CPen* pOld = pDC->SelectObject(&theApp.PenGrid);
	pDC->Arc(Hsp,Vsp,rect.Width()-Hsp,rect.Height()-Vsp,rect.Width()/2,Vsp,rect.Width()/2,Vsp);
	pDC->Arc(Hsp+R/2,Vsp+R/2,rect.Width()-Hsp-R/2,rect.Height()-Vsp-R/2,rect.Width()/2,Vsp+R/2,rect.Width()/2,Vsp+R/2);
	//pDC->Ellipse(Hsp,Vsp,rect.Width()-Hsp,rect.Height()-Vsp);
	for(int i=0;i<12;i++)
	{
		pDC->MoveTo(center);
		pDC->LineTo(center.x+(int)(R*cos(i*PI/6)),center.y-(int)(R*sin(i*PI/6)));
	}
	//计算
	CursorNum1 =pDoc->CursorNum1;
	CString tempA,tempB,tempC;
	int A,B,C,curPos;
	m_pDlg->ChanA.GetLBText(m_pDlg->ChanA.GetCurSel(),tempA);
	m_pDlg->ChanB.GetLBText(m_pDlg->ChanB.GetCurSel(),tempB);
	m_pDlg->ChanC.GetLBText(m_pDlg->ChanC.GetCurSel(),tempC);
	curPos = 0;
	A = _wtoi((tempA.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	B = _wtoi((tempB.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	C = _wtoi((tempC.Tokenize(_T(":"), curPos).GetBuffer()));
	//pDoc->pFile->GetCurrentSeqComp(CursorNum1+1,A,B,C,1);
	int NA[3];NA[0]=A;NA[1]=B;NA[2]=C;
	double firsttheta = 0.0;
	for (int i=0; i<3; i++)
	{
		int ChanNum= NA[i];
		int Flag = pDoc->pFile->AVector[ChanNum-1]->GetCurrentPhasor(CursorNum1+1,pDoc->pFile->m_SampleRate);
		if (Flag==1)
		{
			pDC->TextOutW(10,10,_T("·该区域无法计算，请将光标后移！"));
			return 0;
		}
		CString uu = pDoc->pFile->AVector[ChanNum-1]->uu;
		double RChan;
		double thetaChan;
		double RChan0 = pDoc->pFile->AVector[ChanNum-1]->APhasor.Amplitude;
		double thetaChan0 = pDoc->pFile->AVector[ChanNum-1]->APhasor.phase;
		//表格部分
		CString CSNo,CSRChan,CSthetaChan;
		CSNo = pDoc->pFile->AVector[ChanNum-1]->ph +_T("相");
		CSRChan.Format(_T("%.3f"),RChan0);
		CSRChan += uu;
		CSthetaChan.Format(_T("%.3f°"),thetaChan0);
		m_pDlg->PDList.InsertItem(i,CSNo);
		m_pDlg->PDList.SetItemText(i,1,CSRChan);
		m_pDlg->PDList.SetItemText(i,2,CSthetaChan);

		//m_pDlg->UpdateData(true);
		//画图部分
		RChan = RChan0 * m_pDlg->Pps;
		if (m_pDlg->m_Mode==1)
		{
			if (i==0)
			{
				firsttheta = thetaChan0;
				thetaChan = 0;
			}
			else thetaChan = thetaChan0 -firsttheta;
		}
		else
			thetaChan = thetaChan0;
		if (i==0)  {pOld = pDC->SelectObject(&theApp.PenA);pDC->SetTextColor(Yellow);}
		else if (i==1)  {pOld = pDC->SelectObject(&theApp.PenB);pDC->SetTextColor(Green);}
		else if (i==2)  {pOld = pDC->SelectObject(&theApp.PenC);pDC->SetTextColor(Red);}
		pDC->MoveTo(center);
		pDC->LineTo(center.x+(int)(RChan*cos(thetaChan*PI/180.0)),center.y-(int)(RChan*sin(thetaChan*PI/180.0)));
		pDC->LineTo(center.x+(int)(RChan*0.9*cos((thetaChan-3)*PI/180.0)),center.y-(int)(RChan*0.9*sin((thetaChan-3)*PI/180.0)));
		pDC->MoveTo(center.x+(int)(RChan*cos(thetaChan*PI/180.0)),center.y-(int)(RChan*sin(thetaChan*PI/180.0)));
		pDC->LineTo(center.x+(int)(RChan*0.9*cos((thetaChan+3)*PI/180.0)),center.y-(int)(RChan*0.9*sin((thetaChan+3)*PI/180.0)));
		CString OutText;
		OutText.Format(_T(" [%d]"),ChanNum);
		pDC->TextOutW(center.x+(int)((RChan+15)*cos(thetaChan*PI/180.0)),center.y-(int)((RChan+15)*sin(thetaChan*PI/180.0)),OutText);
		pDC->SelectObject(pOld);
	}
	return 0;
}


int CSeqSView::DrawS(CDC* pDC)
{
	m_pDlg->SDList.DeleteAllItems();
	CRect rect;
	using splab::PI;
	extern CFA2013App theApp;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect,RGB(0,0,0));//背景
	//pDC->SetBkColor(RGB(0,0,0));
	CFont* def_font = pDC->SelectObject(&theApp.font);
	pDC->SetTextColor(White);
	pDC->SetBkMode(TRANSPARENT);
	int Vsp = (int)(rect.Height()*0.05);
	int R = (int)(rect.Height()*0.45);
	int Hsp = (int)(rect.Width()/2) - R;
	pDC->TextOutW(10,rect.Height()-20,_T("序分量图"));
	CPoint center(rect.Width()/2,rect.Height()/2);
	CPen* pOld = pDC->SelectObject(&theApp.PenGrid);
	pDC->Arc(Hsp,Vsp,rect.Width()-Hsp,rect.Height()-Vsp,rect.Width()/2,Vsp,rect.Width()/2,Vsp);
	pDC->Arc(Hsp+R/2,Vsp+R/2,rect.Width()-Hsp-R/2,rect.Height()-Vsp-R/2,rect.Width()/2,Vsp+R/2,rect.Width()/2,Vsp+R/2);
	//pDC->Ellipse(Hsp,Vsp,rect.Width()-Hsp,rect.Height()-Vsp);
	for(int i=0;i<12;i++)
	{
		pDC->MoveTo(center);
		pDC->LineTo(center.x+(int)(R*cos(i*PI/6)),center.y-(int)(R*sin(i*PI/6)));
	}
	//计算
	CursorNum1 =pDoc->CursorNum1;
	CString tempA,tempB,tempC;
	int A,B,C,curPos;
	m_pDlg->ChanA.GetLBText(m_pDlg->ChanA.GetCurSel(),tempA);
	m_pDlg->ChanB.GetLBText(m_pDlg->ChanB.GetCurSel(),tempB);
	m_pDlg->ChanC.GetLBText(m_pDlg->ChanC.GetCurSel(),tempC);
	curPos = 0;
	A = _wtoi((tempA.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	B = _wtoi((tempB.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	C = _wtoi((tempC.Tokenize(_T(":"), curPos).GetBuffer()));
	int Flag = pDoc->pFile->GetCurrentSeqComp(CursorNum1+1,A,B,C,1);
	if (Flag==1)
	{
		pDC->TextOutW(10,10,_T("·该区域无法计算，请将光标后移！"));
		return 0;
	}
	double firsttheta = 0.0;
	for (int i=0; i<3; i++)
	{
		CString uu = pDoc->pFile->AVector[A-1]->uu;
		double RChan;
		double thetaChan;
		double RChan0;
		double thetaChan0;
		CString CSNo,CSRChan,CSthetaChan;
		if (i==0)
		{
			RChan0 = abs(pDoc->pFile->CurentSeqComp[0].PSC);
			thetaChan0 = arg(pDoc->pFile->CurentSeqComp[0].PSC);  //弧度
			CSNo = _T("正序分量");
		}
		else if (i==1)
		{
			RChan0 = abs(pDoc->pFile->CurentSeqComp[0].NSC);
			thetaChan0 = arg(pDoc->pFile->CurentSeqComp[0].NSC);    //弧度
			CSNo = _T("负序分量");
		}
		else if (i==2)
		{
			RChan0 = abs(pDoc->pFile->CurentSeqComp[0].ZSC);
			thetaChan0 = arg(pDoc->pFile->CurentSeqComp[0].ZSC);    //弧度
			CSNo = _T("零序分量");
		}
		//表格部分
		//CSNo见上面
		CSRChan.Format(_T("%.3f"),RChan0);
		CSRChan += uu;
		CSthetaChan.Format(_T("%.3f°"),thetaChan0*180/PI);
		m_pDlg->SDList.InsertItem(i,CSNo);
		m_pDlg->SDList.SetItemText(i,1,CSRChan);
		m_pDlg->SDList.SetItemText(i,2,CSthetaChan);

		//m_pDlg->UpdateData(true);
		//画图部分
		RChan = RChan0 * m_pDlg->Sps;
		if (m_pDlg->m_Mode==1)
		{
			if (i==0)
			{
				firsttheta = thetaChan0;
				thetaChan = 0;
			}
			else thetaChan = thetaChan0 -firsttheta;
		}
		else
			thetaChan = thetaChan0;
		if (i==0)  {pOld = pDC->SelectObject(&theApp.PenA);pDC->SetTextColor(Yellow);}
		else if (i==1)  {pOld = pDC->SelectObject(&theApp.PenB);pDC->SetTextColor(Green);}
		else if (i==2)  {pOld = pDC->SelectObject(&theApp.PenN);pDC->SetTextColor(White);}
		pDC->MoveTo(center);
		pDC->LineTo(center.x+(int)(RChan*cos(thetaChan)),center.y-(int)(RChan*sin(thetaChan)));
		pDC->LineTo(center.x+(int)(RChan*0.9*cos((thetaChan-0.06))),center.y-(int)(RChan*0.9*sin((thetaChan-0.06))));
		pDC->MoveTo(center.x+(int)(RChan*cos(thetaChan)),center.y-(int)(RChan*sin(thetaChan)));
		pDC->LineTo(center.x+(int)(RChan*0.9*cos((thetaChan+0.06))),center.y-(int)(RChan*0.9*sin((thetaChan+0.06))));
		CString OutText;
		if (i==0)  OutText = _T("[1]");
		else if (i==1)  OutText = _T("[2]");
		else if (i==2)  OutText = _T("[0]");
		pDC->TextOutW(center.x+(int)((RChan+15)*cos(thetaChan)),center.y-(int)((RChan+15)*sin(thetaChan)),OutText);
		pDC->SelectObject(pOld);
	}
	return 0;
}


void CDlgSeqComp::OnBnClickedPinc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Pps<50)
		Pps =Pps *1.5;
	m_pPView->Invalidate();
}


void CDlgSeqComp::OnBnClickedPdec()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Pps>0.1)
		Pps =Pps /1.5;
	m_pPView->Invalidate();
}


void CDlgSeqComp::OnBnClickedSinc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Sps<50)
		Sps =Sps *1.5;
	m_pSView->Invalidate();
}


void CDlgSeqComp::OnBnClickedSdec()
{
	// TODO: 在此添加控件通知处理程序代码
	if (Sps>0.1)
		Sps =Sps /1.5;
	m_pSView->Invalidate();
}


void CDlgSeqComp::OnBnClickedPini()
{
	// TODO: 在此添加控件通知处理程序代码
	Pps = 1.0;
	m_pPView->Invalidate();
}


void CDlgSeqComp::OnBnClickedSini()
{
	// TODO: 在此添加控件通知处理程序代码
	Sps = 1.0;
	m_pSView->Invalidate();
}


void CDlgSeqComp::OnBnClickedTurnmode()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pWnd = GetDlgItem(IDC_TurnMode);
	if (m_Mode ==0)
	{
		m_Mode =1;
		pWnd->SetWindowTextW(_T("转为旋转模式"));
	}
	else
	{
		m_Mode =0;
		pWnd->SetWindowTextW(_T("转为相对模式"));
	}

	m_pPView->Invalidate();
	m_pSView->Invalidate();
}


void CDlgSeqComp::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	DestroyWindow();
	//CDialog::OnCancel();
}


void CDlgSeqComp::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->pDoc->pDlgSeqComp = NULL;
	delete this;
	CDialog::PostNcDestroy();
}


void CDlgSeqComp::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	//CDialog::OnClose();
}


int CSeqPView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	//return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


int CSeqSView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	//return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
