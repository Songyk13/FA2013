
// FA2013.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "FA2013.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "FA2013Doc.h"
#include "AView.h"
#include "DView.h"
#include "ThreadFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFA2013App

BEGIN_MESSAGE_MAP(CFA2013App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CFA2013App::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
//	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
ON_COMMAND(ID_FILE_OPEN, &CFA2013App::OnFileOpen)
END_MESSAGE_MAP()


// CFA2013App 构造

CFA2013App::CFA2013App()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("FA2013.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	PenA.CreatePen(PS_SOLID,1,Yellow);//黄
	PenB.CreatePen(PS_SOLID,1,Green);//绿
	PenC.CreatePen(PS_SOLID,1,Red);//红
	PenN.CreatePen(PS_SOLID,1,White);//白
	PenF.CreatePen(PS_SOLID,1,Orange);//橙
	PenAD.CreatePen(PS_DASH,1,Yellow);
	PenBD.CreatePen(PS_DASH,1,Green);//绿
	PenCD.CreatePen(PS_DASH,1,Red);//红
	
	PenV.CreatePen(PS_SOLID,1,Virtual);
	PenCur.CreatePen(PS_SOLID,2,RGB(187,255,255));
	BrushA.CreateSolidBrush(Yellow);
	BrushB.CreateSolidBrush(Green);
	BrushC.CreateSolidBrush(Red);
	BrushN.CreateSolidBrush(White);
	BrushF.CreateSolidBrush(Orange);
	BrushV.CreateSolidBrush(Virtual);
	BrushBk.CreateSolidBrush(RGB(0x38,0x8E,0x8E));

	PenGridGray.CreatePen(PS_SOLID,1,Gray);//灰
	PenGrid.CreatePen(PS_DASH,1,Gray);
	PenAxis1.CreatePen(PS_SOLID,1,Pink);//粉红实线
	PenAxis2.CreatePen(PS_DASH,1,Orange);//橙色虚线

	APenG1.CreatePen(PS_SOLID,1,Gray00);
	APenG2.CreatePen(PS_SOLID,1,Gray0);
	APenG3.CreatePen(PS_SOLID,1,Gray1);

	Pen0.CreatePen(PS_SOLID,1,Green);
	Pen1.CreatePen(PS_SOLID,1,Red);

	VERIFY(font.CreateFont(
	20,                        // nHeight
	0,                         // nWidth
	0,                         // nEscapement
	0,                         // nOrientation
	FW_NORMAL,                 // nWeight
	FALSE,                     // bItalic
	FALSE,                     // bUnderline
	0,                         // cStrikeOut
	ANSI_CHARSET,              // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	_T("微软雅黑")));                 // lpszFacename

	sfont.CreateFont(
	16,                        // nHeight
	0,                         // nWidth
	0,                         // nEscapement
	0,                         // nOrientation
	FW_NORMAL,                 // nWeight
	FALSE,                     // bItalic
	FALSE,                     // bUnderline
	0,                         // cStrikeOut
	ANSI_CHARSET,              // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	_T("微软雅黑"));

}

// 唯一的一个 CFA2013App 对象

CFA2013App theApp;


// CFA2013App 初始化

BOOL CFA2013App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_FA2013TYPE,
		RUNTIME_CLASS(CFA2013Doc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CAView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	//AddDocTemplate (new CMultiDocTemplate(IDR_FA2013TYPE, 
 //         RUNTIME_CLASS(CFA2013Doc), 
 //         RUNTIME_CLASS(CChildFrame), 
 //         RUNTIME_CLASS(CDView))); 

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();

	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CFA2013App::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CFA2013App 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CFA2013App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CFA2013App 自定义加载/保存方法

void CFA2013App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CFA2013App::LoadCustomState()
{
}

void CFA2013App::SaveCustomState()
{
}

// CFA2013App 消息处理程序





void CFA2013App::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CWinAppEx::OnFileNew();
	CMainFrame  *pMain = (CMainFrame*) AfxGetMainWnd();
	CChildFrame *pChild = (CChildFrame*)(pMain->GetActiveFrame());
	CFA2013Doc *pDoc = (CFA2013Doc*)(pChild->GetActiveDocument());
	
	CFileDialog *IpzsOpenFile;
	CString szGetName;
	IpzsOpenFile = new CFileDialog(true,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST 
		|OFN_HIDEREADONLY,
		TEXT("文件类型(*.cfg;*.dat)|*.cfg;*.dat||"));//生成一个对话框
	if(IpzsOpenFile->DoModal() == IDOK)//假如点击对话框确定按
	{
		szGetName = IpzsOpenFile->GetPathName();//得到打开文件的路径
		CString FileName = szGetName.Left(szGetName.GetLength()-4);
		pDoc->pFile = new CComtrade(FileName);
		delete IpzsOpenFile;
	}
	else
	{
		delete IpzsOpenFile;
		return;
	}
	//释放分配的对话框
//	Flag_import=1;
	pDoc->UpdateAllViews(NULL);
	//pDoc->OnFileOpen();
	pDoc->pThread = AfxBeginThread(ThreadFunc,pDoc);

}
