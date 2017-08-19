#pragma once


// CDlgBusDiffData 对话框

class CDlgBusDiffData : public CDialog
{
	DECLARE_DYNAMIC(CDlgBusDiffData)

public:
	CDlgBusDiffData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBusDiffData();

// 对话框数据
	enum { IDD = IDD_BusDiffData };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double Im;
	double Iop;
	double Iopres;
	double Kres;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
