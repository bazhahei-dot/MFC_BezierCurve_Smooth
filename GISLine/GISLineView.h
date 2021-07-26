
// GISLineView.h: CGISLineView 类的接口
//

#pragma once
#include"MyPoint.h"
#include"MyLine.h"
#include"CTestDlg.h"
#include "GISLineDoc.h"
#include<vector>
#include "CMyBezier.h"
class CGISLineView : public CView
{
protected: // 仅从序列化创建
	CGISLineView() noexcept;
	DECLARE_DYNCREATE(CGISLineView)

// 特性
public:
	CGISLineDoc* GetDocument() const;
	CTestDlg* m_pTestDlg;
	int Height, Width;
	bool m_bButtonDown = false;
	float Scale;
	int m_nX;
	int m_nY;
	double m_Scale = 1;
	double m_dMaxError=0;
	double m_dSumError = 0;
	double m_dAveError;
	vector<vector<Point>> Beziers;
	double m_dBezierThreshold;
	int m_nBezierWidth;
	int m_nBezierLocation;
	COLORREF m_cBezierColor;
// 操作
public:
	CPoint m_Point[24263], m_OriginPoint[24263], m_pCentrePoint,m_pStartPoint, m_pEndPoint;
	double m_nScale;
	int m_nDeltaX, m_nDeltaY;
	int m_nFlag;
	int m_nRectHeight;
	int m_nRectWidth;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CGISLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawline();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnBigger();
	afx_msg void On32782();
	afx_msg void OnBezierClickedOk();
	void GetCtlPoint(CPoint point1, CPoint point2);
	afx_msg void OnCal();
	// 贝塞尔平滑的阈值
	double m_dThreshold;
	afx_msg void OnOnbigger();
	afx_msg void OnOnsmaller();
	afx_msg void OnMove();
	afx_msg void OnOnrectangle();
	// 贝塞尔曲线生成
	void MyBezierCurve();
};

#ifndef _DEBUG  // GISLineView.cpp 中的调试版本
inline CGISLineDoc* CGISLineView::GetDocument() const
   { return reinterpret_cast<CGISLineDoc*>(m_pDocument); }
#endif

