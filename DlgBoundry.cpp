// DlgBoundry.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgBoundry.h"
#include "afxdialogex.h"
#include "Boundry.h"
//#include "DlgBdChild.h"
//
//class CDlgBdParam;
//class CDlgBdPoly;
//class CDlgBdLine;
//class CDlgBdCircle;
//class CDlgBdRect;
// CDlgBoundry 对话框

IMPLEMENT_DYNAMIC(CDlgBoundry, CDialogEx)

CDlgBoundry::CDlgBoundry(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBoundry::IDD, pParent)
	, m_pDlg(NULL)
{

}

CDlgBoundry::~CDlgBoundry()
{
}

void CDlgBoundry::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BdTab, m_tab);
	DDX_Control(pDX, IDC_BdChild, m_static);
	DDX_Control(pDX, IDC_BdList1, BdListCtrl);
}


BEGIN_MESSAGE_MAP(CDlgBoundry, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_BdTab, &CDlgBoundry::OnTcnSelchangeBdtab)
	ON_NOTIFY(NM_CLICK, IDC_BdList1, &CDlgBoundry::OnNMClickBdlist1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_BdList1, &CDlgBoundry::OnLvnItemchangedBdlist1)
	ON_NOTIFY(LVN_INSERTITEM, IDC_BdList1, &CDlgBoundry::OnLvnInsertitemBdlist1)
	ON_BN_CLICKED(IDC_DeleteBd, &CDlgBoundry::OnBnClickedDeleteBd)
	ON_BN_CLICKED(IDC_ClearBd, &CDlgBoundry::OnBnClickedClearBd)
END_MESSAGE_MAP()


// CDlgBoundry 消息处理程序


BOOL CDlgBoundry::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_static.ShowWindow(SW_HIDE);
	m_tab.InsertItem(0,_T("绘图参数"));
	m_tab.InsertItem(1,_T("多边形"));
	m_tab.InsertItem(2,_T("直线"));
	m_tab.InsertItem(3,_T("圆形"));
	m_tab.InsertItem(4,_T("四边形"));
	BdParam.Create(IDD_BdParam,this);
	BdPoly.Create(IDD_BdPoly,this);
	BdPoly.m_pDlg=m_pDlg;
	BdPoly.outerDlg=this;
	BdLine.Create(IDD_BdLine,this);
	BdLine.m_pDlg=m_pDlg;
	BdLine.outerDlg=this;
	BdCircle.Create(IDD_BdCircle,this);
	BdCircle.m_pDlg=m_pDlg;
	BdCircle.outerDlg=this;
	BdRect.Create(IDD_BdRect,this);
	BdRect.m_pDlg=m_pDlg;
	BdRect.outerDlg=this;
	CRect rect;
	//m_tab.GetClientRect(rect);
	GetDlgItem(IDC_BdChild)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	//rect.bottom += 0;

	BdParam.MoveWindow(rect);
	BdPoly.MoveWindow(rect);
	BdLine.MoveWindow(rect);
	BdCircle.MoveWindow(rect);
	BdRect.MoveWindow(rect);

	BdParam.ShowWindow(SW_SHOW);
	BdPoly.ShowWindow(SW_HIDE);
	BdLine.ShowWindow(SW_HIDE);
	BdCircle.ShowWindow(SW_HIDE);
	BdRect.ShowWindow(SW_HIDE);

	//初始化边界列表
	CRect listRect;
	//设置扩展风格
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(BdListCtrl.m_hWnd, styles, styles );
	BdListCtrl.GetClientRect(&listRect);
	BdListCtrl.InsertColumn(0,_T("边界"),LVCFMT_LEFT,listRect.Width()/5*4,-1);
	//BdListCtrl.InsertColumn(1,_T("通道"),LVCFMT_LEFT,listRect.Width()/4*3,-1);
	int totalNum = m_pDlg->BdList.size();
	for(int i=0; i<totalNum;i++)
	{
		BdListCtrl.InsertItem(LVIF_TEXT | LVIF_STATE,i,m_pDlg->BdList[i]->Name,0,LVIS_SELECTED,0,0);
		//BdListCtrl.SetItemText(i,1,pDoc->pFile->AVector[i]->name);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgBoundry::OnTcnSelchangeBdtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_tab.GetCurSel())
	{
	case 0:
		{
			m_pDlg->t_Boundry.isVisible = 0;
			BdParam.ShowWindow(SW_SHOW);
			BdPoly.ShowWindow(SW_HIDE);
			BdLine.ShowWindow(SW_HIDE);
			BdCircle.ShowWindow(SW_HIDE);
			BdRect.ShowWindow(SW_HIDE);
			break;
		}
	case 1:
		{
			m_pDlg->t_Boundry.isVisible = 0;
			BdParam.ShowWindow(SW_HIDE);
			BdPoly.ShowWindow(SW_SHOW);
			BdLine.ShowWindow(SW_HIDE);
			BdCircle.ShowWindow(SW_HIDE);
			BdRect.ShowWindow(SW_HIDE);
			break;
		}
	case 2:
		{
			m_pDlg->t_Boundry.isVisible = 0;
			BdParam.ShowWindow(SW_HIDE);
			BdPoly.ShowWindow(SW_HIDE);
			BdLine.ShowWindow(SW_SHOW);
			BdCircle.ShowWindow(SW_HIDE);
			BdRect.ShowWindow(SW_HIDE);
			break;
		}
	case 3:
		{
			m_pDlg->t_Boundry.isVisible = 0;
			BdParam.ShowWindow(SW_HIDE);
			BdPoly.ShowWindow(SW_HIDE);
			BdLine.ShowWindow(SW_HIDE);
			BdCircle.ShowWindow(SW_SHOW);
			BdRect.ShowWindow(SW_HIDE);
			break;
		}
	case 4:
		{
			m_pDlg->t_Boundry.isVisible = 0;
			BdParam.ShowWindow(SW_HIDE);
			BdPoly.ShowWindow(SW_HIDE);
			BdLine.ShowWindow(SW_HIDE);
			BdCircle.ShowWindow(SW_HIDE);
			BdRect.ShowWindow(SW_SHOW);
			break;
		}
	}
	*pResult = 0;
}


int CDlgBoundry::refresh_BdList(void)
{
	BdListCtrl.DeleteAllItems();
	int totalNum = m_pDlg->BdList.size();
	for(int i=0; i<totalNum;i++)
	{
		BdListCtrl.InsertItem(LVIF_TEXT | LVIF_STATE,i,m_pDlg->BdList[i]->Name,0,LVIS_SELECTED,0,0);
		//BdListCtrl.SetItemText(i,1,pDoc->pFile->AVector[i]->name);
	}
	return 0;
}


void CDlgBoundry::OnNMClickBdlist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString str1;
	int i = pNMListView->iItem;//行
	if (i>=m_pDlg->BdList.size())
	{
		*pResult = 0;
		return;
	}
	CBoundry* tBd = m_pDlg->BdList[i];
	switch(tBd->Type[0])
	{
	case 0:
		{
			m_tab.SetCurSel(0);
			BdParam.ShowWindow(SW_SHOW);
			BdPoly.ShowWindow(SW_HIDE);
			BdLine.ShowWindow(SW_HIDE);
			BdCircle.ShowWindow(SW_HIDE);
			BdRect.ShowWindow(SW_HIDE);
			break;
		}
	case _T('P'):
		{
			m_tab.SetCurSel(1);
			m_pDlg->t_Boundry.Rset1 = tBd->Rset1;
			m_pDlg->t_Boundry.Xset1 = tBd->Xset1;
			m_pDlg->t_Boundry.Beta = tBd->Beta;
			m_pDlg->t_Boundry.Alpha = tBd->Alpha;
			m_pDlg->t_Boundry.Theta = tBd->Theta;
			m_pDlg->t_Boundry.Type = tBd->Type;
			m_pDlg->t_Boundry.isVisible = 1;
			BdPoly.m_pView->m_pDlg=m_pDlg;
			BdPoly.UpdateData(FALSE);
			//BdPoly.m_pView->Invalidate();
			//BdPoly.m_pView->UpdateWindow();
			BdParam.ShowWindow(SW_HIDE);
			BdPoly.ShowWindow(SW_SHOW);
			BdLine.ShowWindow(SW_HIDE);
			BdCircle.ShowWindow(SW_HIDE);
			BdRect.ShowWindow(SW_HIDE);
			break;
		}
	case _T('L'):
		{
			m_tab.SetCurSel(2);
			m_pDlg->t_Boundry.Rset1 = tBd->Rset1;
			m_pDlg->t_Boundry.Xset1 = tBd->Xset1;
			m_pDlg->t_Boundry.Beta = tBd->Beta;
			m_pDlg->t_Boundry.Type = tBd->Type;
			m_pDlg->t_Boundry.isVisible = 1;
			BdLine.m_pView->m_pDlg=m_pDlg;
			BdLine.UpdateData(FALSE);
			BdParam.ShowWindow(SW_HIDE);
			BdPoly.ShowWindow(SW_HIDE);
			BdLine.ShowWindow(SW_SHOW);
			BdCircle.ShowWindow(SW_HIDE);
			BdRect.ShowWindow(SW_HIDE);
			break;
		}
	case _T('C'):
		{
			m_tab.SetCurSel(3);
			m_pDlg->t_Boundry.Rset1 = tBd->Rset1;
			m_pDlg->t_Boundry.Xset1 = tBd->Xset1;
			m_pDlg->t_Boundry.Alpha = tBd->Alpha;
			m_pDlg->t_Boundry.Type = tBd->Type;
			m_pDlg->t_Boundry.isVisible = 1;
			BdCircle.m_pView->m_pDlg=m_pDlg;
			BdCircle.UpdateData(FALSE);
			BdParam.ShowWindow(SW_HIDE);
			BdPoly.ShowWindow(SW_HIDE);
			BdLine.ShowWindow(SW_HIDE);
			BdCircle.ShowWindow(SW_SHOW);
			BdRect.ShowWindow(SW_HIDE);
			break;
		}
	case _T('R'):
		{
			m_tab.SetCurSel(4);
			m_pDlg->t_Boundry.Rset1 = tBd->Rset1;
			m_pDlg->t_Boundry.Xset1 = tBd->Xset1;
			m_pDlg->t_Boundry.Rset2 = tBd->Rset2;
			m_pDlg->t_Boundry.Xset2 = tBd->Xset2;
			m_pDlg->t_Boundry.Type = tBd->Type;
			m_pDlg->t_Boundry.isVisible = 1;
			BdRect.m_pView->m_pDlg=m_pDlg;
			BdRect.UpdateData(FALSE);
			BdParam.ShowWindow(SW_HIDE);
			BdPoly.ShowWindow(SW_HIDE);
			BdLine.ShowWindow(SW_HIDE);
			BdCircle.ShowWindow(SW_HIDE);
			BdRect.ShowWindow(SW_SHOW);
			break;
		}
	}
	*pResult = 0;
}


void CDlgBoundry::OnLvnItemchangedBdlist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//m_pDlg->refresh_BdList();
	*pResult = 0;
}


void CDlgBoundry::OnLvnInsertitemBdlist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_pDlg->refresh_BdList();
	*pResult = 0;
}


void CDlgBoundry::OnBnClickedDeleteBd()
{
	std::vector<CBoundry*>::iterator iter = m_pDlg->BdList.begin();
	for(int i=0; i<BdListCtrl.GetItemCount(); ++i,++iter)
	{
		if(BdListCtrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			delete m_pDlg->BdList[i];
			m_pDlg->BdList.erase(iter);
			refresh_BdList();
			m_pDlg->refresh_BdList();
			m_pDlg->m_pView->Invalidate();
			m_pDlg->m_pView->UpdateWindow();
			break;
		}
	}
}


void CDlgBoundry::OnBnClickedClearBd()
{
	for(int i=0; i<BdListCtrl.GetItemCount(); ++i)
	{
		delete m_pDlg->BdList[i];
	}
	BdListCtrl.DeleteAllItems();
	m_pDlg->BdList.clear();
	m_pDlg->BdListOuter.DeleteAllItems();

	m_pDlg->m_pView->Invalidate();
	m_pDlg->m_pView->UpdateWindow();
	m_pDlg->L_num = 1;
	m_pDlg->P_num = 1;
	m_pDlg->R_num = 1;
	m_pDlg->C_num = 1;
}
