// DlgBusDiffData.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgBusDiffData.h"
#include "afxdialogex.h"


// CDlgBusDiffData 对话框

IMPLEMENT_DYNAMIC(CDlgBusDiffData, CDialog)

CDlgBusDiffData::CDlgBusDiffData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBusDiffData::IDD, pParent)
	, Im(0)
	, Iop(0)
	, Iopres(0)
	, Kres(0)
{

}

CDlgBusDiffData::~CDlgBusDiffData()
{
}

void CDlgBusDiffData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Im, Im);
	DDX_Text(pDX, IDC_Iop, Iop);
	DDX_Text(pDX, IDC_Iresop, Iopres);
	DDX_Text(pDX, IDC_Kres, Kres);
}


BEGIN_MESSAGE_MAP(CDlgBusDiffData, CDialog)
END_MESSAGE_MAP()


// CDlgBusDiffData 消息处理程序


BOOL CDlgBusDiffData::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	Im=8;
	Iop=0.3;
	Iopres=1.2;
	Kres =0.8;
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgBusDiffData::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(true);
	CDialog::OnOK();
}
