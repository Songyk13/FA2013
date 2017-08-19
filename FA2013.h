
// FA2013.h : FA2013 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号


// CFA2013App:
// 有关此类的实现，请参阅 FA2013.cpp
//

class CFA2013App : public CWinAppEx
{
public:
	CFA2013App();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	CPen PenA;//黄
	CPen PenB;//绿
	CPen PenC;//红
	CPen PenN;//白
	CPen PenF;//橙

	CPen PenAD;//黄
	CPen PenBD;//绿
	CPen PenCD;//红

	CPen PenV;
	CPen PenCur;
	CBrush BrushA;
	CBrush BrushB;
	CBrush BrushC;
	CBrush BrushN;
	CBrush BrushF;
	CBrush BrushV;
	CBrush BrushBk;

	//Axis
	CPen APenG1;
	CPen APenG2;
	CPen APenG3;

	CPen PenGridGray;//灰
	CPen PenGrid;//虚灰线
	CPen PenAxis1;//粉红实线
	CPen PenAxis2;//橙色虚线
	
	CPen Pen0;//D通道0
	CPen Pen1;//D通道1

	CFont font;
	CFont sfont;
	afx_msg void OnFileOpen();
};

extern CFA2013App theApp;
