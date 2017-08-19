#pragma once
#include "FA2013Doc.h"
#include "afxwin.h"
class CFA2013Doc;
class CBusDiffView;
class CDlgBusDiff;

// CDlgBusDiff 对话框

class CDlgBusDiff : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBusDiff)

public:
	CDlgBusDiff(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBusDiff();

// 对话框数据
	enum { IDD = IDD_BusDiff };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CFA2013Doc* pDoc;
	CBusDiffView* m_pView;
	virtual BOOL OnInitDialog();
	CComboBox Ia1List;
	CComboBox Ib1List;
	CComboBox Ic1List;
	CComboBox Ia2List;
	CComboBox Ib2List;
	CComboBox Ic2List;
	CComboBox BDTypeList;
	double Im,Iop,Iopres,Kres,Iresm;
	int LocFlag;//轨迹
	int Ia1,Ia2,Ib1,Ib2,Ic1,Ic2;
	afx_msg void OnCbnSelchangeIa1list();
	afx_msg void OnCbnSelchangeIb1list();
	afx_msg void OnCbnSelchangeIc1list();
	afx_msg void OnCbnSelchangeIa2list();
	afx_msg void OnCbnSelchangeIb2list();
	afx_msg void OnCbnSelchangeIc2list();
	afx_msg void OnBnClickedLocvisable();
	afx_msg void OnBnClickeddiinc();
	afx_msg void OnBnClickeddidec();
	afx_msg void OnBnClickeddirst();
	afx_msg void OnCbnSelchangeBusdifftype();
	afx_msg void OnBnClickedSetdata();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnClose();
};
#pragma once


// CBusDiffView 视图

class CBusDiffView : public CView
{
	DECLARE_DYNCREATE(CBusDiffView)

protected:
	CBusDiffView();           // 动态创建所使用的受保护的构造函数
	virtual ~CBusDiffView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	CFA2013Doc* pDoc;
	CDlgBusDiff* m_pDlg;
	CPoint MidP;
	int DrawBusDiff(CDC* pDC);
	double SPD,IPS;
	double DefaultSPD,DefaultIPS;
	int CursorNum1;
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


