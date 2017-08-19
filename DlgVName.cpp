// DlgVName.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgVName.h"
#include "afxdialogex.h"


// CDlgVName 对话框

IMPLEMENT_DYNAMIC(CDlgVName, CDialog)

CDlgVName::CDlgVName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVName::IDD, pParent)
	, t_VName(_T(""))
{

}

CDlgVName::~CDlgVName()
{
}

void CDlgVName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VNAMEEDIT, VNameEdit);
}


BEGIN_MESSAGE_MAP(CDlgVName, CDialog)
END_MESSAGE_MAP()


// CDlgVName 消息处理程序


void CDlgVName::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	int len = VNameEdit.LineLength(VNameEdit.LineIndex(0));
	VNameEdit.GetLine(0,t_VName.GetBuffer(len), len);
	t_VName.ReleaseBuffer(len);
	CDialog::OnOK();
}
