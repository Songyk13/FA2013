#pragma once
#include "afxwin.h"
#include "Comtrade.h"
#include "FA2013Doc.h"

// CDlgHarmo 对话框
class CHarmoView;
class CFA2013Doc;
class CDlgHarmo : public CDialog
{
	DECLARE_DYNAMIC(CDlgHarmo)

public:
	CDlgHarmo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHarmo();

// 对话框数据
	enum { IDD = IDD_Harmonics };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox Alist;
	CFA2013Doc* pDoc;
	afx_msg void OnLbnSelchangeAlist();
	CHarmoView* m_pView;
//	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnClose();
};
#pragma once


// CHarmoView 视图

class CHarmoView : public CView
{
	DECLARE_DYNCREATE(CHarmoView)

protected:
	CHarmoView();           // 动态创建所使用的受保护的构造函数
	virtual ~CHarmoView();

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
	int CursorNum1;
	int ChanNum1;
	int DrawHarmo(CDC* pDC);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


