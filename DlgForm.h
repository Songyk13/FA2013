#pragma once
#include "afxwin.h"
#include "Comtrade.h"
#include "FA2013Doc.h"
#include "afxcmn.h"
#include <vector>
#include <map>

// DlgForm 对话框
class CVView;
class CFA2013Doc;

class CDlgForm : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgForm)

public:
	CDlgForm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgForm();

// 对话框数据
	enum { IDD = IDD_Formula };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CFA2013Doc* pDoc;
	CListCtrl AList;
	CListCtrl VList;
	CListCtrl ConstList;
	CListCtrl FxList;

	CEdit Input;
	CEdit Status;

	afx_msg void OnNMDblclkChanlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkConlist(NMHDR *pNMHDR, LRESULT *pResult);
	CVView* m_pView;
	int initConst(std::map<CString, double>* con);
	afx_msg void OnBnClickedGen();
	AChannel* t_VChannel;
	int initFunc(std::map<CString,CString>* fx,std::map<CString,CString>* des);
	afx_msg void OnBnClickedInsert();
	CString t_VName;
	afx_msg void OnNMDblclkFunlist(NMHDR *pNMHDR, LRESULT *pResult);
	int refresh_VList(void);
	afx_msg void OnNMClickVlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusVlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMHoverFunlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickFunlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusFunlist(NMHDR *pNMHDR, LRESULT *pResult);
};
#pragma once



// CVView 视图

class CVView : public CScrollView
{
	DECLARE_DYNCREATE(CVView)

protected:
	CVView();           // 动态创建所使用的受保护的构造函数
	virtual ~CVView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void OnInitialUpdate();     // 构造后的第一次

	DECLARE_MESSAGE_MAP()

public:
	CFA2013Doc* pDoc;
	int DrawVirtual(CDC* pDC);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CDlgForm* m_pDlg;
};


