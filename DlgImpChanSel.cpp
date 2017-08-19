// DlgImpChanSel.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgImpChanSel.h"
#include "afxdialogex.h"


// CDlgImpChanSel 对话框

IMPLEMENT_DYNAMIC(CDlgImpChanSel, CDialogEx)

CDlgImpChanSel::CDlgImpChanSel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgImpChanSel::IDD, pParent)
	, pDoc(NULL)
	, Type(0)
	, Ua(-1)
	, Ub(-1)
	, Uc(-1)
	, Ia(-1)
	, Ib(-1)
	, Ic(-1)
	, SelFlag(0)
	, R1(0)
	, X1(0)
	, R0(0)
	, X0(0)
{

}

CDlgImpChanSel::~CDlgImpChanSel()
{
}

void CDlgImpChanSel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ChanUa, ChanUaS);
	DDX_Control(pDX, IDC_ChanUb, ChanUbS);
	DDX_Control(pDX, IDC_ChanUc, ChanUcS);
	DDX_Control(pDX, IDC_ChanIa, ChanIaS);
	DDX_Control(pDX, IDC_ChanIb, ChanIbS);
	DDX_Control(pDX, IDC_ChanIc, ChanIcS);

	DDX_Text(pDX, IDC_ZR1, R1);
	DDX_Text(pDX, IDC_ZX1, X1);
	DDX_Text(pDX, IDC_ZR0, R0);
	DDX_Text(pDX, IDC_ZX0, X0);
}


BEGIN_MESSAGE_MAP(CDlgImpChanSel, CDialogEx)
	ON_CBN_SELCHANGE(IDC_ChanUa, &CDlgImpChanSel::OnCbnSelchangeChanua)
	ON_CBN_SELCHANGE(IDC_ChanUb, &CDlgImpChanSel::OnCbnSelchangeChanub)
	ON_CBN_SELCHANGE(IDC_ChanUc, &CDlgImpChanSel::OnCbnSelchangeChanuc)
	ON_CBN_SELCHANGE(IDC_ChanIa, &CDlgImpChanSel::OnCbnSelchangeChania)
	ON_CBN_SELCHANGE(IDC_ChanIb, &CDlgImpChanSel::OnCbnSelchangeChanib)
	ON_CBN_SELCHANGE(IDC_ChanIc, &CDlgImpChanSel::OnCbnSelchangeChanic)
END_MESSAGE_MAP()


// CDlgImpChanSel 消息处理程序


BOOL CDlgImpChanSel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (Type == 1)//接地阻抗
	{
		R1=0.01976;
		X1=0.2801;
		R0=0.1406;
		X0=0.6471;
		UpdateData(false);
		CWnd* pWnd = GetDlgItem(IDC_ZR1);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_ZX1);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_ZR0);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_ZX0);
		pWnd->EnableWindow(TRUE);
	}
	else
	{
		R1=0.01976;
		X1=0.2801;
		R0=0.1406;
		X0=0.6471;
		UpdateData(false);
		CWnd* pWnd = GetDlgItem(IDC_ZR1);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_ZX1);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_ZR0);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_ZX0);
		pWnd->EnableWindow(TRUE);
	}
	int uA,uB,uC,iA,iB,iC;
	uA=uB=uC=iA=iB=iC=0;
	for(int i=0; i<pDoc->pFile->m_ANumber;i++)
	{
		CString tempCS;
		tempCS.Format(_T("%d:"),i+1);
		tempCS += pDoc->pFile->AVector[i]->name;
		CString tempUU = pDoc->pFile->AVector[i]->uu;
		if ((tempUU == _T("kV"))||(tempUU == _T("V")))
		{
			if ((pDoc->pFile->AVector[i]->ph == _T("A"))||(pDoc->pFile->AVector[i]->ph == _T("a")))
				ChanUaS.InsertString(uA++,tempCS);
			else if ((pDoc->pFile->AVector[i]->ph == _T("B"))||(pDoc->pFile->AVector[i]->ph == _T("b")))
				ChanUbS.InsertString(uB++,tempCS);
			else if ((pDoc->pFile->AVector[i]->ph == _T("C"))||(pDoc->pFile->AVector[i]->ph == _T("c")))
				ChanUcS.InsertString(uC++,tempCS);
		}
		else if ((tempUU == _T("kA"))||(tempUU == _T("A")))
		{
			if ((pDoc->pFile->AVector[i]->ph == _T("A"))||(pDoc->pFile->AVector[i]->ph == _T("a")))
				ChanIaS.InsertString(iA++,tempCS);
			else if ((pDoc->pFile->AVector[i]->ph == _T("B"))||(pDoc->pFile->AVector[i]->ph == _T("b")))
				ChanIbS.InsertString(iB++,tempCS);
			else if ((pDoc->pFile->AVector[i]->ph == _T("C"))||(pDoc->pFile->AVector[i]->ph == _T("c")))
				ChanIcS.InsertString(iC++,tempCS);
		}
	}
	ChanUaS.SetCurSel(0);
	ChanUbS.SetCurSel(0);
	ChanUcS.SetCurSel(0);
	ChanIaS.SetCurSel(0);
	ChanIbS.SetCurSel(0);
	ChanIcS.SetCurSel(0);
	CDlgImpChanSel::OnCbnSelchangeChanub();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgImpChanSel::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	SelFlag = 1;
	UpdateData(true);
	CDialogEx::OnOK();
}


void CDlgImpChanSel::OnCbnSelchangeChanua()
{
	// TODO: 在此添加控件通知处理程序代码
	//自动填充
	int a,curPos;
	CString temp,tempA,tempBC;
	ChanUaS.GetLBText(ChanUaS.GetCurSel(),temp);
	curPos=0;
	a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
	tempA = pDoc->pFile->AVector[a-1]->ccbm;
	int numB = ChanUbS.GetCount();
	int numC = ChanUcS.GetCount();
	int curB=0;
	int curC=0;
	while(curB<numB)
	{
		ChanUbS.SetCurSel(curB);
		ChanUbS.GetLBText(ChanUbS.GetCurSel(),temp);
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
		ChanUcS.SetCurSel(curC);
		ChanUcS.GetLBText(ChanUcS.GetCurSel(),temp);
		curPos=0;
	    a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
		tempBC = pDoc->pFile->AVector[a-1]->ccbm;
		if (tempBC==tempA)
			break;
		else
			curC++;
	}
	//自动填充完毕
	CDlgImpChanSel::OnCbnSelchangeChanub();
}


void CDlgImpChanSel::OnCbnSelchangeChanub()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tempUA,tempUB,tempUC,tempIA,tempIB,tempIC,tempU,tempI,tempUCS,tempICS;
	int UA,UB,UC,IA,IB,IC,curPos;
	ChanUaS.GetLBText(ChanUaS.GetCurSel(),tempUA);
	ChanUbS.GetLBText(ChanUbS.GetCurSel(),tempUB);
	ChanUcS.GetLBText(ChanUcS.GetCurSel(),tempUC);
	ChanIaS.GetLBText(ChanIaS.GetCurSel(),tempIA);
	ChanIbS.GetLBText(ChanIbS.GetCurSel(),tempIB);
	ChanIcS.GetLBText(ChanIcS.GetCurSel(),tempIC);
	curPos = 0;
	Ua = UA = _wtoi((tempUA.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	Ub = UB = _wtoi((tempUB.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	Uc = UC = _wtoi((tempUC.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	Ia = IA = _wtoi((tempIA.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	Ib = IB = _wtoi((tempIB.Tokenize(_T(":"), curPos).GetBuffer()));
	curPos = 0;
	Ic = IC = _wtoi((tempIC.Tokenize(_T(":"), curPos).GetBuffer()));
	tempU = pDoc->pFile->AVector[UA-1]->ccbm;
	tempI = pDoc->pFile->AVector[IA-1]->ccbm;
	CWnd* pWnd = GetDlgItem(IDC_ChanSelStatus);
	if ((pDoc->pFile->AVector[UB-1]->ccbm == tempU)&&(pDoc->pFile->AVector[UC-1]->ccbm == tempU))
		tempUCS = _T("电压通道:")+tempU;
	else
		tempUCS = _T("电压通道:无意义的组合。");
	if ((pDoc->pFile->AVector[IB-1]->ccbm == tempI)&&(pDoc->pFile->AVector[IC-1]->ccbm == tempI))
		tempICS = _T("电流通道:")+tempI;
	else
		tempICS = _T("电流通道:无意义的组合。");
	pWnd->SetWindowTextW(tempUCS+ _T("\n") +tempICS +_T("\n请选择相同元件的A、B、C三相通道。"));
}


void CDlgImpChanSel::OnCbnSelchangeChanuc()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgImpChanSel::OnCbnSelchangeChanub();
}


void CDlgImpChanSel::OnCbnSelchangeChania()
{
	// TODO: 在此添加控件通知处理程序代码
	//自动填充
	int a,curPos;
	CString temp,tempA,tempBC;
	ChanIaS.GetLBText(ChanIaS.GetCurSel(),temp);
	curPos=0;
	a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
	tempA = pDoc->pFile->AVector[a-1]->ccbm;
	int numB = ChanIbS.GetCount();
	int numC = ChanIcS.GetCount();
	int curB=0;
	int curC=0;
	while(curB<numB)
	{
		ChanIbS.SetCurSel(curB);
		ChanIbS.GetLBText(ChanIbS.GetCurSel(),temp);
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
		ChanIcS.SetCurSel(curC);
		ChanIcS.GetLBText(ChanIcS.GetCurSel(),temp);
		curPos=0;
	    a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
		tempBC = pDoc->pFile->AVector[a-1]->ccbm;
		if (tempBC==tempA)
			break;
		else
			curC++;
	}
	//自动填充完毕
	CDlgImpChanSel::OnCbnSelchangeChanub();
}


void CDlgImpChanSel::OnCbnSelchangeChanib()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgImpChanSel::OnCbnSelchangeChanub();
}


void CDlgImpChanSel::OnCbnSelchangeChanic()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgImpChanSel::OnCbnSelchangeChanub();
}
