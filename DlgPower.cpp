// DlgPower.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgPower.h"
#include "afxdialogex.h"


// CDlgPower 对话框

IMPLEMENT_DYNAMIC(CDlgPower, CDialogEx)

CDlgPower::CDlgPower(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPower::IDD, pParent)
, pDoc(NULL)
, Ua(-1)
, Ub(-1)
, Uc(-1)
, Ia(-1)
, Ib(-1)
, Ic(-1)
{

}

CDlgPower::~CDlgPower()
{
}

void CDlgPower::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PUa, ChanUa);
	DDX_Control(pDX, IDC_PUb, ChanUb);
	DDX_Control(pDX, IDC_PUc, ChanUc);
	DDX_Control(pDX, IDC_PIa, ChanIa);
	DDX_Control(pDX, IDC_PIb, ChanIb);
	DDX_Control(pDX, IDC_PIc, ChanIc);
}


BEGIN_MESSAGE_MAP(CDlgPower, CDialogEx)
	ON_CBN_SELCHANGE(IDC_PUa, &CDlgPower::OnCbnSelchangePua)
	ON_CBN_SELCHANGE(IDC_PUb, &CDlgPower::OnCbnSelchangePub)
	ON_CBN_SELCHANGE(IDC_PUc, &CDlgPower::OnCbnSelchangePuc)
	ON_CBN_SELCHANGE(IDC_PIa, &CDlgPower::OnCbnSelchangePia)
	ON_CBN_SELCHANGE(IDC_PIb, &CDlgPower::OnCbnSelchangePib)
	ON_CBN_SELCHANGE(IDC_PIc, &CDlgPower::OnCbnSelchangePic)
END_MESSAGE_MAP()


// CDlgPower 消息处理程序


BOOL CDlgPower::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
				ChanUa.InsertString(uA++,tempCS);
			else if ((pDoc->pFile->AVector[i]->ph == _T("B"))||(pDoc->pFile->AVector[i]->ph == _T("b")))
				ChanUb.InsertString(uB++,tempCS);
			else if ((pDoc->pFile->AVector[i]->ph == _T("C"))||(pDoc->pFile->AVector[i]->ph == _T("c")))
				ChanUc.InsertString(uC++,tempCS);
		}
		else if ((tempUU == _T("kA"))||(tempUU == _T("A")))
		{
			if ((pDoc->pFile->AVector[i]->ph == _T("A"))||(pDoc->pFile->AVector[i]->ph == _T("a")))
				ChanIa.InsertString(iA++,tempCS);
			else if ((pDoc->pFile->AVector[i]->ph == _T("B"))||(pDoc->pFile->AVector[i]->ph == _T("b")))
				ChanIb.InsertString(iB++,tempCS);
			else if ((pDoc->pFile->AVector[i]->ph == _T("C"))||(pDoc->pFile->AVector[i]->ph == _T("c")))
				ChanIc.InsertString(iC++,tempCS);
		}
	}
	ChanUa.SetCurSel(0);
	ChanUb.SetCurSel(0);
	ChanUc.SetCurSel(0);
	ChanIa.SetCurSel(0);
	ChanIb.SetCurSel(0);
	ChanIc.SetCurSel(0);
	CDlgPower::OnCbnSelchangePuc();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgPower::OnCbnSelchangePua()
{
	// TODO: 在此添加控件通知处理程序代码
	//自动填充
	int a,curPos;
	CString temp,tempA,tempBC;
	ChanUa.GetLBText(ChanUa.GetCurSel(),temp);
	curPos=0;
	a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
	tempA = pDoc->pFile->AVector[a-1]->ccbm;
	int numB = ChanUb.GetCount();
	int numC = ChanUc.GetCount();
	int curB=0;
	int curC=0;
	while(curB<numB)
	{
		ChanUb.SetCurSel(curB);
		ChanUb.GetLBText(ChanUb.GetCurSel(),temp);
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
		ChanUc.SetCurSel(curC);
		ChanUc.GetLBText(ChanUc.GetCurSel(),temp);
		curPos=0;
	    a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
		tempBC = pDoc->pFile->AVector[a-1]->ccbm;
		if (tempBC==tempA)
			break;
		else
			curC++;
	}
	//自动填充完毕
	CDlgPower::OnCbnSelchangePuc();


}


void CDlgPower::OnCbnSelchangePub()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgPower::OnCbnSelchangePuc();
}


void CDlgPower::OnCbnSelchangePuc()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tempUA,tempUB,tempUC,tempIA,tempIB,tempIC,tempU,tempI,tempUCS,tempICS;
	int UA,UB,UC,IA,IB,IC,curPos;
	ChanUa.GetLBText(ChanUa.GetCurSel(),tempUA);
	ChanUb.GetLBText(ChanUb.GetCurSel(),tempUB);
	ChanUc.GetLBText(ChanUc.GetCurSel(),tempUC);
	ChanIa.GetLBText(ChanIa.GetCurSel(),tempIA);
	ChanIb.GetLBText(ChanIb.GetCurSel(),tempIB);
	ChanIc.GetLBText(ChanIc.GetCurSel(),tempIC);
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
	CWnd* pWnd = GetDlgItem(IDC_PowerSelStatus);
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


void CDlgPower::OnCbnSelchangePia()
{
	// TODO: 在此添加控件通知处理程序代码
	//自动填充
	int a,curPos;
	CString temp,tempA,tempBC;
	ChanIa.GetLBText(ChanIa.GetCurSel(),temp);
	curPos=0;
	a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
	tempA = pDoc->pFile->AVector[a-1]->ccbm;
	int numB = ChanIb.GetCount();
	int numC = ChanIc.GetCount();
	int curB=0;
	int curC=0;
	while(curB<numB)
	{
		ChanIb.SetCurSel(curB);
		ChanIb.GetLBText(ChanIb.GetCurSel(),temp);
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
		ChanIc.SetCurSel(curC);
		ChanIc.GetLBText(ChanIc.GetCurSel(),temp);
		curPos=0;
	    a = _wtoi((temp.Tokenize(_T(":"), curPos).GetBuffer()));
		tempBC = pDoc->pFile->AVector[a-1]->ccbm;
		if (tempBC==tempA)
			break;
		else
			curC++;
	}
	//自动填充完毕
	CDlgPower::OnCbnSelchangePuc();
}


void CDlgPower::OnCbnSelchangePib()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgPower::OnCbnSelchangePuc();
}


void CDlgPower::OnCbnSelchangePic()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgPower::OnCbnSelchangePuc();
}
