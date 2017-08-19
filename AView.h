
// AView.h : CAView 类的接口
//

#pragma once
#include"uniclass.h"
#include "stdafx.h"
#include"FA2013Doc.h"

class CAView : public CScrollView
{
protected: // 仅从序列化创建
	CAView();
	DECLARE_DYNCREATE(CAView)

// 特性
public:
	CFA2013Doc* GetDocument() const;

// 操作
public:
	int drawCom(CDC* m_DC);
	int drawMark(CDC* pDC,CPoint pPos, int up, int down);
	double getSampleRate(int SamplePoint,std::vector<SampleRate>& m_SampleRate);
	int drawAxis(CDC* pDC, CPoint pPos, int startSample, int endSample, CComtrade* pFile);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CAView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
//	afx_msg void OnFilePrintPreview();
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnReset();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // AView.cpp 中的调试版本
inline CFA2013Doc* CAView::GetDocument() const
   { return reinterpret_cast<CFA2013Doc*>(m_pDocument); }
#endif

