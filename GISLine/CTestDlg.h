#pragma once


// CTestDlg 对话框

class CTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTestDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 平滑阈值控件
	CComboBox m_cbThreshold;
	int dataWidth[3],dataLocation[3];
	double dataThreshold[5];
	afx_msg void OnBnClickedOk();
	// 线条宽度
	CComboBox m_cbWidth;
	// 显示位置
	CComboBox m_cdLocation;
	// 颜色选择
	CMFCColorButton m_cbColor;
//	afx_msg void OnBnClickedMfccolorbutton1();
};
