#pragma once
#include "afxcmn.h"
#include "dlgbdchild.h"
#include "afxwin.h"
#include "DlgBdChild.h"

class CDlgBdParam;
class CDlgBdPoly;
class CDlgBdLine;
class CDlgBdCircle;
class CDlgBdRect;

// CDlgBoundry 对话框

class CDlgBoundry : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBoundry)

public:
	CDlgBoundry(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBoundry();

// 对话框数据
	enum { IDD = IDD_Boundry };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeBdtab(NMHDR *pNMHDR, LRESULT *pResult);

	CDlgBdParam BdParam;
	CDlgBdPoly BdPoly;
	CDlgBdLine BdLine;
	CDlgBdCircle BdCircle;
	CDlgBdRect BdRect;
	CStatic m_static;
	CDlgImp* m_pDlg;
	CListCtrl BdListCtrl;
	int refresh_BdList(void);
	afx_msg void OnNMClickBdlist1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedBdlist1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnInsertitemBdlist1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDeleteBd();
	afx_msg void OnBnClickedClearBd();
};
