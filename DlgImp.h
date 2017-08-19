#pragma once
#include "FA2013Doc.h"
#include "afxwin.h"
#include "atltypes.h"
#include "Boundry.h"
#include <vector>
#include "afxcmn.h"

class CFA2013Doc;
class CDlgImp;
class CImpView;
// CDlgImp 对话框

class CDlgImp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImp)

public:
	CDlgImp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgImp();

// 对话框数据
	enum { IDD = IDD_Impedance };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFA2013Doc* pDoc;
	CImpView* m_pView;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSetbd();
	CComboBox ImpTypeSel;
	afx_msg void OnCbnSelchangeImptype();
	int Ua,Ub,Uc,Ia,Ib,Ic;//选择的通道初始化为-1。
	int SelFlag;     //通道是否已选择 1:已选择，0:未选择
	LineData LD;
	std::vector<CBoundry*> BdList;    //边界列表
	unsigned int P_num,L_num,C_num,R_num;  //边界计数
	CBoundry t_Boundry;               //预览用临时边界
	CPoint t_midP;

	afx_msg void OnBnClickedImprst();
	afx_msg void OnBnClickedDisploc();
	CListCtrl BdListOuter;
	afx_msg void OnNMClickBdlist(NMHDR *pNMHDR, LRESULT *pResult);
	int refresh_BdList(void);
	afx_msg void OnLvnItemchangedBdlist(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl ImpList;
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
#pragma once


// CImpView 视图

class CImpView : public CView
{
	DECLARE_DYNCREATE(CImpView)

protected:
	CImpView();           // 动态创建所使用的受保护的构造函数
	virtual ~CImpView();

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
	CDlgImp* m_pDlg;
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	
	int DrawImp(CDC* pDC);
	// 鼠标点击位置
	CPoint MPos;
	CPoint MidP,MidPLast,MidP0;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	double defaultZPS;   //Z/像素（默认）
	double ZPS;          //Z/像素
	double defaultSPD;   //像素/刻度（默认）
	double SPD;          //像素/刻度
	int ZoomFlag;
	int LocFlag;   //是否显示轨迹 0不显示 1显示
	int CursorNum1;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


