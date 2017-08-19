// DlgForm.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgForm.h"
#include "afxdialogex.h"
#include "uniclass.h"
#include "calc.h"
#include "AChannel.h"
#include <math.h>
#include <algorithm>
#include <numeric>
#include "DlgVName.h"

// DlgForm 对话框
//std::map<CString,double> Constants;
extern std::map<CString,double> Constants;
extern std::map<CString,CString> Funcs;
extern std::map<CString,CString> FxDes;
class CDlgform;
class CDlgVName;

IMPLEMENT_DYNAMIC(CDlgForm, CDialogEx)

CDlgForm::CDlgForm(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgForm::IDD, pParent)
	, m_pView(NULL)
	, t_VChannel(NULL)
	, t_VName(_T(""))
{

}

CDlgForm::~CDlgForm()
{
}

void CDlgForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ChanList, AList);
	DDX_Control(pDX, IDC_EDIT1, Input);
	DDX_Control(pDX, IDC_EDIT2, Status);
	DDX_Control(pDX, IDC_VList, VList);
	DDX_Control(pDX, IDC_ConList, ConstList);
	DDX_Control(pDX, IDC_FunList, FxList);
}


BEGIN_MESSAGE_MAP(CDlgForm, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_ChanList, &CDlgForm::OnNMDblclkChanlist)
	ON_NOTIFY(NM_DBLCLK, IDC_ConList, &CDlgForm::OnNMDblclkConlist)
	ON_BN_CLICKED(IDC_GEN, &CDlgForm::OnBnClickedGen)
	ON_BN_CLICKED(IDC_INSERT, &CDlgForm::OnBnClickedInsert)
	ON_NOTIFY(NM_DBLCLK, IDC_FunList, &CDlgForm::OnNMDblclkFunlist)
	ON_NOTIFY(NM_CLICK, IDC_VList, &CDlgForm::OnNMClickVlist)
	ON_NOTIFY(NM_KILLFOCUS, IDC_VList, &CDlgForm::OnNMKillfocusVlist)
	ON_NOTIFY(NM_HOVER, IDC_FunList, &CDlgForm::OnNMHoverFunlist)
	ON_NOTIFY(NM_CLICK, IDC_FunList, &CDlgForm::OnNMClickFunlist)
	ON_NOTIFY(NM_KILLFOCUS, IDC_FunList, &CDlgForm::OnNMKillfocusFunlist)
END_MESSAGE_MAP()


// DlgForm 消息处理程序


BOOL CDlgForm::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Status.SetWindowTextW(_T("就绪。"));

	CString str1,str2;
	int n;
	CRect listRect;
	//设置扩展风格
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	//初始化通道列表
	ListView_SetExtendedListViewStyleEx(AList.m_hWnd, styles, styles );
	AList.GetClientRect(&listRect);
	AList.InsertColumn(0,_T("序号"),LVCFMT_LEFT,listRect.Width()/4,-1);
	AList.InsertColumn(1,_T("通道"),LVCFMT_LEFT,listRect.Width()/4*3,-1);
	int m_ANumber = pDoc->pFile->m_ANumber;
	for(int i=0; i<m_ANumber;i++)
	{
		str1.Format(_T("A%d"),i+1);
		AList.InsertItem(LVIF_TEXT | LVIF_STATE,i,str1,0,LVIS_SELECTED,0,0);
		AList.SetItemText(i,1,pDoc->pFile->AVector[i]->name);
	}
	//初始化虚拟通道列表
	ListView_SetExtendedListViewStyleEx(VList.m_hWnd, styles, styles );
	VList.GetClientRect(&listRect);
	VList.InsertColumn(0,_T("序号"),LVCFMT_LEFT,listRect.Width()/4,-1);
	VList.InsertColumn(1,_T("虚拟通道"),LVCFMT_LEFT,listRect.Width()/4*3,-1);
	int m_VNumber = pDoc->pFile->m_VNumber;
	for(int i=0; i<m_VNumber;i++)
	{
		str1.Format(_T("V%d"),i+1);
		VList.InsertItem(LVIF_TEXT | LVIF_STATE,i,str1,0,LVIS_SELECTED,0,0);
		VList.SetItemText(i,1,pDoc->pFile->VVector[i]->name);
	}
	//初始化常量列表
	initConst(&Constants);
	ListView_SetExtendedListViewStyleEx(ConstList.m_hWnd, styles, styles );
	ConstList.GetClientRect(&listRect);
	ConstList.InsertColumn(0,_T("常量"),LVCFMT_LEFT,listRect.Width()/4,-1);
	ConstList.InsertColumn(1,_T("值"),LVCFMT_LEFT,listRect.Width()/4*3,-1);
	std::map<CString, double>::iterator it_Con = Constants.begin();
	for(int i=0; it_Con != Constants.end(); ++it_Con ,++i)
	{
		ConstList.InsertItem(LVIF_TEXT | LVIF_STATE,i,it_Con->first,0,LVIS_SELECTED,0,0);
		str2.Format(_T("%f"),it_Con->second);
		ConstList.SetItemText(i,1,str2);
	}
	//初始化函数列表
	initFunc(&Funcs,&FxDes);
	ListView_SetExtendedListViewStyleEx(FxList.m_hWnd, styles, styles );
	FxList.GetClientRect(&listRect);
	FxList.InsertColumn(0,_T("函数"),LVCFMT_LEFT,listRect.Width()/3,-1);
	FxList.InsertColumn(1,_T("功能"),LVCFMT_LEFT,listRect.Width()/3*2,-1);
	std::map<CString,CString>::iterator it_Fx = Funcs.begin();
	for(int i=0; it_Fx != Funcs.end(); ++it_Fx ,++i)
	{
		n = _wtoi(it_Fx->second.Right(1));
		str1 = it_Fx->first+_T("()");
		//for(int j=0; i<n; ++j)
		//{
		//}
		FxList.InsertItem(LVIF_TEXT | LVIF_STATE,i,str1,0,LVIS_SELECTED,0,0);
		str2 = it_Fx->second.Left(it_Fx->second.GetLength()-1);
		FxList.SetItemText(i,1,str2);
	}
	//
	////////////////////////////////////////////////////////////////

	//创建视图用于显示通道
	UINT TargetCtrlID = IDC_VVIEW;  
	CWnd *pWnd = this->GetDlgItem(TargetCtrlID);  
	CRect RectTargetCtrl;  
	pWnd->GetWindowRect(RectTargetCtrl);  
	this->ScreenToClient(RectTargetCtrl);  
	m_pView = (CVView*)RUNTIME_CLASS(CVView)->CreateObject();
	(m_pView->pDoc)=pDoc;
	m_pView->m_pDlg=this;
	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = 3000;
	sizeTotal.cy = RectTargetCtrl.Height();
	m_pView->SetScrollSizes(MM_TEXT, sizeTotal);
	//在目标位置动态创建视图
	if (NULL==m_pView)  
	{  
	return FALSE;  
	}  
	m_pView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, RectTargetCtrl, this, TargetCtrlID);

	GetDlgItem (IDC_INPUT)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgForm::OnNMDblclkChanlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	int i = 0;
	CString str1;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	i = pNMListView->iItem;//行
	str1.Format(_T("A%d"),i+1);
	Input.ReplaceSel(str1,TRUE);
	Input.SetFocus();
	*pResult = 0;
}

void CDlgForm::OnNMDblclkConlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	int i = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	i = pNMListView->iItem;//行
	CString str1(ConstList.GetItemText(i,0));
	Input.ReplaceSel(str1,TRUE);
	Input.SetFocus();
	*pResult = 0;
}

void CDlgForm::OnNMDblclkFunlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int i = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	i = pNMListView->iItem;//行

	CString str1(FxList.GetItemText(i,0));
	str1.Remove(_T('('));
	str1.Remove(_T(')'));
	int n = _wtoi(Funcs[str1].Right(1));
	if(n==0)
	{
		n=1;
	}

	str1 += _T('(');
	for(int j=1; j<n; ++j)
	{
		str1+=_T(',');
	}
	str1 += _T(')');

	Input.ReplaceSel(str1,TRUE);
	int s,e;
	Input.SetFocus();
	Input.GetSel(s,e);
	Input.SetSel(s-n,e-n,FALSE);
	*pResult = 0;
}
// E:\fa_\FA2013\DlgForm.cpp : 实现文件
//

#include "stdafx.h"
#include "FA2013.h"
#include "DlgForm.h"


// CVView

IMPLEMENT_DYNCREATE(CVView, CScrollView)

CVView::CVView()
: m_pDlg(NULL)
{

}

CVView::~CVView()
{
}


BEGIN_MESSAGE_MAP(CVView, CScrollView)
	ON_WM_MOUSEACTIVATE()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CVView 绘图

void CVView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	CSize bar;
	// TODO: 计算此视图的合计大小
	sizeTotal = this->GetTotalSize();
	//GetScrollBarSizes(bar);
	//sizeTotal.cy -= bar.cx;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CVView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	CDC m_DC;
	if(!m_DC.CreateCompatibleDC(NULL))				
	{														
		::PostQuitMessage(0);
	}
	this->DrawVirtual(&m_DC);
	m_DC.DeleteDC();
	// TODO: 在此添加绘制代码
}


// CVView 诊断

#ifdef _DEBUG
void CVView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CVView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CVView 消息处理程序


int CVView::DrawVirtual(CDC* m_DC)
{
	CRect bkgRect;
	this->GetClientRect(bkgRect);
	CPoint pPoint = this->GetScrollPosition();
	CSize realSize;
	int startSample, endSample;
	int lSpace = 20;
	CDC* pDC= this->GetDC();
	CBitmap m_Bmp;
	m_Bmp.CreateCompatibleBitmap(pDC, bkgRect.Width(), bkgRect.Height());
	CBitmap* bOld = (CBitmap*)(m_DC->SelectObject(&m_Bmp));

	realSize.cx = (LONG)((m_pDlg->pDoc->pFile->m_TotalSampleNum)*
		(m_pDlg->pDoc->pFile->m_drawParam.SPScale))+lSpace;
	realSize.cy = bkgRect.Height();
	SetScrollSizes(MM_TEXT,realSize);

	m_DC->DPtoLP(&bkgRect);
	m_DC->FillSolidRect(bkgRect,RGB(0,0,0));
	if(NULL!=m_pDlg->t_VChannel)
	{
		startSample=(int)(pPoint.x/(m_pDlg->pDoc->pFile->m_drawParam.SPScale));
		endSample=(int)((bkgRect.Width()+pPoint.x)/(m_pDlg->pDoc->pFile->m_drawParam.SPScale));
		m_pDlg->t_VChannel->drawChannel(m_DC,CPoint(lSpace,bkgRect.Height()/2),startSample,endSample,m_pDlg->pDoc->pFile->m_drawParam);
		//标最大值
		CFont* def_font = m_DC->SelectObject(&theApp.sfont);
		m_DC->SetTextColor(Yellow);
		m_DC->SetBkMode(TRANSPARENT);
		CString max1,max2;
		max1.Format(_T("%.2f"),m_pDlg->t_VChannel->maxAData);
		max2.Format(_T("-%.2f"),m_pDlg->t_VChannel->maxAData);
		m_DC->TextOutW(0,0,max1);
		m_DC->TextOutW(0,bkgRect.Height()-15,max2);
		m_DC->SelectObject(def_font);
	}
	else
	{
		m_DC->FillSolidRect(bkgRect,RGB(0,0,0));
	}
	pDC->BitBlt(0, 0, bkgRect.Width(), bkgRect.Height(), m_DC, 0, 0, SRCCOPY);
	m_DC->SelectObject(bOld);
	m_Bmp.DeleteObject();
	m_DC->DeleteDC();
	ReleaseDC(pDC);
	return 0;
}


int CVView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//return CScrollView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


int CDlgForm::initConst(std::map<CString, double>* con)
{
	std::map<CString, double>* Constants = con;
	(*Constants)[_T("PI")]=3.14159265;
	(*Constants)[_T("EXP")]=2.71828183;
	(*Constants)[_T("SQRT2")]=1.41421356;
	(*Constants)[_T("SQRT3")]=1.73205080;
	return 0;
}

int CDlgForm::initFunc(std::map<CString,CString>* fx,std::map<CString,CString>* des)
{
	(*fx)[_T("abs")]="取绝对值。1";
	(*des)[_T("abs")]="接受一个参数。";
	(*fx)[_T("sin")]="取正弦值。1";
	(*des)[_T("sin")]="接受一个参数。";
	
	(*fx)[_T("harm_amp")]="取谐波幅值。2";
	(*des)[_T("harm_amp")]="接受两个参数：通道，次数。";
	(*fx)[_T("basewave")]="取基波波形/幅值/相角。2";
	(*des)[_T("basewave")]="接受两个参数：通道，类型：1-波形，2-幅值，3-相角。";
	(*fx)[_T("imp")]="取阻抗实部/虚部/模值/相角。3";
	(*des)[_T("imp")]="接受三个参数：电压，电流，类型：1-实部，2-虚部，3-模值，4-相角。";
	(*fx)[_T("power_p")]="取一对或多对通道有功功率之和。0";
	(*des)[_T("power_p")]="接受偶数个参数，依次为电压、电流、电压、电流...";
	(*fx)[_T("power_q")]="取一对或多对通道无功功率之和。0";
	(*des)[_T("power_q")]="接受偶数个参数，依次为电压、电流、电压、电流...";
	(*fx)[_T("power_s")]="取一对或多对通道视在功率之和。0";
	(*des)[_T("power_s")]="接受偶数个参数，依次为电压、电流、电压、电流...";
	(*fx)[_T("make_sine")]="生成标准正弦波。3";
	(*des)[_T("make_sine")]="接受三个参数：幅值，初相，频率。";
	(*fx)[_T("make_complex")]="生成复数。3";
	(*des)[_T("make_complex")]="接受三个参数，第三个为类型，若为1则输入实部，虚部；若为2则输入幅值和相角（角度）。";
	(*fx)[_T("complex")]="求复数的实部/虚部/模值/相角。2";
	(*des)[_T("complex")]="接受二个参数，第一个为通道，第二个为类型：1-实部，2-虚部，3-模值，4-相角。";
	return 0;
}

void CVView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnUpdate(NULL,0,NULL);
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}



BOOL CVView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CScrollView::OnEraseBkgnd(pDC);
}


void CDlgForm::OnBnClickedGen()
{
	CString input;
	CRect bkgRect;
	m_pView->GetClientRect(bkgRect);
	int len = Input.LineLength(Input.LineIndex(0));
	Input.GetLine(0, input.GetBuffer(len), len);
	input.ReleaseBuffer(len);

	CString result;
	if(exam(&result,input, pDoc->pFile))
	{
		t_VChannel = NULL;
		Status.SetWindowTextW(result);
		m_pView->Invalidate();
		m_pView->UpdateWindow();
		return;
	}

	if(t_VChannel == NULL)
		t_VChannel = new AChannel;
	else
	{
		//delete t_VChannel;
		t_VChannel = new AChannel;
	}

	input.Insert(0,_T("("));
	input+=_T(")");

	*t_VChannel = EVA(input, pDoc->pFile);

	if(t_VChannel->uu == _T("ERR"))
	{
		result = _T("公式解析过程中出现错误，请检查公式。");
		Status.SetWindowTextW(result);
		delete t_VChannel;
		t_VChannel = NULL;
		m_pView->Invalidate();
		m_pView->UpdateWindow();
		return;
	}

	t_VChannel->isReal = 0;
	t_VChannel->uu = input;
	t_VChannel->ph=_T("V");

	//生成通道绘图参数
	int VSpace = (int)(bkgRect.Height()*0.95);
	double max1,max2;
	t_VChannel->m_drawPara.VSpace=VSpace;
	t_VChannel->m_drawPara.defultVSpace=VSpace;
	max1 = *max_element(t_VChannel->AData.begin(),t_VChannel->AData.end());
	max2 = -*min_element(t_VChannel->AData.begin(),t_VChannel->AData.end());
	max1 = (max1>max2)?max1:max2;
	t_VChannel->maxAData = max1;
	t_VChannel->m_drawPara.defaultUPScale=max1/(VSpace/2);
	t_VChannel->m_drawPara.UPScale=t_VChannel->m_drawPara.defaultUPScale;
	t_VChannel->name=_T("");
	///////////////////
	result = _T("公式解析成功。");
	Status.SetWindowTextW(result);
	m_pView->Invalidate();
	m_pView->UpdateWindow();
	// TODO: 在此添加控件通知处理程序代码
}



void CDlgForm::OnBnClickedInsert()
{
	if (NULL == t_VChannel)
	{
		MessageBox(_T("请先生成虚拟通道再插入！"),_T("错误")); 
	}
	else
	{
		CDlgVName vnDlg;
		if(vnDlg.DoModal() == IDOK)
		{
			t_VName = vnDlg.t_VName;
			t_VChannel->name = t_VName;
			//t_VChannel->ph=_T("V");
			pDoc->pFile->VVector.push_back(t_VChannel);
			pDoc->pFile->m_VNumber += 1;
			refresh_VList();

			pDoc->pFile->AVector.push_back(t_VChannel);
			pDoc->pFile->m_ANumber += 1;
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
		}
	}
	// TODO: 在此添加控件通知处理程序代码
}




int CDlgForm::refresh_VList(void)
{
	CString str1;
	int m_VNumber = pDoc->pFile->m_VNumber;
	VList.DeleteAllItems();
	for(int i=0; i<m_VNumber;i++)
	{
		str1.Format(_T("V%d"),i+1);
		VList.InsertItem(LVIF_TEXT | LVIF_STATE,i,str1,0,LVIS_SELECTED,0,0);
		VList.SetItemText(i,1,pDoc->pFile->VVector[i]->name);
	}
	return 0;
}


void CDlgForm::OnNMClickVlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString str1;
	int i = pNMListView->iItem;//行
	t_VChannel = pDoc->pFile->VVector[i];
	str1 = _T("正在显示虚拟通道： “");
	str1 += pDoc->pFile->VVector[i]->name + _T("”；");
	str1 += _T("\r\n公式为： ");
	str1 += pDoc->pFile->VVector[i]->uu + _T(" 。");
	Status.SetWindowTextW(str1);
	m_pView->Invalidate();
	m_pView->UpdateWindow();
	*pResult = 0;
}


void CDlgForm::OnNMKillfocusVlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	Status.SetWindowTextW(_T("就绪。"));
	t_VChannel = NULL;
	m_pView->Invalidate();
	m_pView->UpdateWindow();
	*pResult = 0;
}


void CDlgForm::OnNMHoverFunlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDlgForm::OnNMClickFunlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString str;
	int i = pNMListView->iItem;//行

	CString str1(FxList.GetItemText(i,0));
	str1.Remove(_T('('));
	str1.Remove(_T(')'));
	str = FxDes[str1];
	Status.SetWindowTextW(str);

	*pResult = 0;
}


void CDlgForm::OnNMKillfocusFunlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	Status.SetWindowTextW(_T("就绪。"));
	*pResult = 0;
}
