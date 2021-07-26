// CTestDlg.cpp: 实现文件
//

#include "pch.h"
#include "GISLine.h"
#include "CTestDlg.h"
#include "afxdialogex.h"
#include "GISLineView.h"
#include "MainFrm.h"


// CTestDlg 对话框

IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	double Threshold = 1.0;
	for (int i = 0; i < 5; i++)
	{
		dataThreshold[i] = Threshold;
		Threshold += 0.5;
	}
	int Width = 1;
	for (int i = 0; i < 3; i++)
	{
		dataWidth[i] = Width;
		Width += 1;
	}
	dataLocation[0] = 400;
	dataLocation[1] = -400;
	dataLocation[2] = 0;
}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO4, m_cbThreshold);
	DDX_Control(pDX, IDC_COMBO5, m_cbWidth);
	DDX_Control(pDX, IDC_COMBO6, m_cdLocation);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_cbColor);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTestDlg::OnBnClickedOk)
//	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &CTestDlg::OnBnClickedMfccolorbutton1)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序


void CTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	CGISLineView* pView = (CGISLineView*)pFrame->GetActiveView();
	int ThresholdIndex = m_cbThreshold.GetCurSel();
	int WidthIndex = m_cbWidth.GetCurSel();
	int LocationIndex = m_cdLocation.GetCurSel();
	COLORREF color = m_cbColor.GetColor();
	pView->m_dBezierThreshold = dataThreshold[ThresholdIndex];
	pView->m_nBezierWidth = dataWidth[WidthIndex];
	pView->m_nBezierLocation = dataLocation[LocationIndex];
	pView->m_cBezierColor = color;
	pView->MyBezierCurve();
	CDialogEx::OnOK();
}

