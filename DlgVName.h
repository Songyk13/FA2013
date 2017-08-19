#pragma once
#include "afxwin.h"


// CDlgVName 对话框

class CDlgVName : public CDialog
{
	DECLARE_DYNAMIC(CDlgVName)

public:
	CDlgVName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVName();

// 对话框数据
	enum { IDD = IDD_VNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CEdit VNameEdit;
	CString t_VName;
};
