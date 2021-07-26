
// GISLineView.cpp: CGISLineView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "GISLine.h"
#include<vector>
#endif

#include "GISLineDoc.h"
#include "GISLineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

// CGISLineView

IMPLEMENT_DYNCREATE(CGISLineView, CView)

BEGIN_MESSAGE_MAP(CGISLineView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DRAWLINE, &CGISLineView::OnDrawline)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_32782, &CGISLineView::On32782)
	ON_BN_CLICKED(IDOK, &CGISLineView::OnBezierClickedOk)
	ON_COMMAND(ID_32783, &CGISLineView::OnCal)
	ON_COMMAND(ID_OnBigger, &CGISLineView::OnOnbigger)
	ON_COMMAND(ID_OnSmaller, &CGISLineView::OnOnsmaller)
	ON_COMMAND(ID_32797, &CGISLineView::OnMove)
	ON_COMMAND(ID_OnRectAngle, &CGISLineView::OnOnrectangle)
END_MESSAGE_MAP()

// CGISLineView 构造/析构

CGISLineView::CGISLineView() noexcept
{
	// TODO: 在此处添加构造代码
	m_nDeltaX = 0;
	m_nDeltaY = 0;
	m_nFlag = 0;
	m_nScale = 1;
	Height = 1500;
	Width = 700;
}

CGISLineView::~CGISLineView()
{
}

BOOL CGISLineView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGISLineView 绘图

void CGISLineView::OnDraw(CDC* pDC)
{
	CGISLineDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CPen MyPen, * OldPen,BezierPen;
	MyPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	CPoint LinePoint[24261], OldPoint, NewPoint;
	int order, oldorder;
	for (int i = 0; i < pDoc->m_nPointNum - 1; i++)
		{
			OldPen = pDC->SelectObject(&MyPen);
			OldPoint = m_Point[i];
			NewPoint = m_Point[i + 1];
			pDC->MoveTo(OldPoint);
			pDC->LineTo(NewPoint);
			pDC->SelectObject(OldPen);
		}
	if (m_nFlag == 2 && m_bButtonDown == true)
	{

		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(m_pStartPoint.x, m_pStartPoint.y, m_pEndPoint.x, m_pEndPoint.y);
	}
	MyPen.DeleteObject();
	BezierPen.CreatePen(PS_SOLID,m_nBezierWidth,m_cBezierColor);
	CPoint Ctl[4];
	for (int i = 0; i < Beziers.size(); i++)
	{
		
		for (int j = 0; j < 4; j++)
		{			
				Ctl[j].x = (Beziers[i][j].x - 414500) * Scale+m_nBezierLocation;
				Ctl[j].y = (Beziers[i][j].y - 4168900) * Scale;
		}
		pDC->PolyBezier(Ctl, 4);
		pDC->PolyBezierTo(Ctl, 4);
		pDC->SelectObject(BezierPen);
		if (i == Beziers.size() - 1)
		{
			CString outLine=_T("曲线点数：");
			CString str1;
			str1.Format(_T("%d"), Beziers.size());
			outLine += str1;
			pDC->TextOutW(-Height / 2+50, Width /2-50, outLine);

			CString outLine2 = _T("比例尺：1:");
			CString str2;
			str2.Format(_T("%d"), (int)(1/m_Scale*1000));
			outLine2 += str2;
			pDC->TextOutW(-Height /2+50, Width /2-70, outLine2);

			CString outLine3 = _T("压缩比：");
			CString str3;
			double CalRes = (double)Beziers.size() / (double)pDoc->m_nPointNum;
			str3.Format(_T("%f"), CalRes);
			outLine3 += str3;
			pDC->TextOutW(-Height / 2 + 50, Width / 2 - 90, outLine3);

			CString outLine4 = _T("平均误差：");
			CString str4;
			m_dAveError= m_dSumError/Beziers.size();
			str4.Format(_T("%f"), m_dAveError);
			outLine4 += str4;
			pDC->TextOutW(-Height / 2 + 50, Width / 2 - 110, outLine4);

			CString outLine5 = _T("最大误差：");
			CString str5;
			str5.Format(_T("%f"), m_dMaxError);
			outLine5 += str5;
			pDC->TextOutW(-Height / 2 + 50, Width / 2 - 130, outLine5);
		}
	}
	BezierPen.DeleteObject();
	// TODO: 在此处为本机数据添加绘制代码
}


// CGISLineView 打印

BOOL CGISLineView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGISLineView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGISLineView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CGISLineView 诊断

#ifdef _DEBUG
void CGISLineView::AssertValid() const
{
	CView::AssertValid();
}

void CGISLineView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGISLineDoc* CGISLineView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGISLineDoc)));
	return (CGISLineDoc*)m_pDocument;
}
#endif //_DEBUG


// CGISLineView 消息处理程序


void CGISLineView::OnDrawline()
{
	CGISLineDoc* pDoc = GetDocument();
	CDC* pDC = GetDC();
	CRect MyRect;
	GetClientRect(MyRect);
	m_nRectHeight = MyRect.bottom;
	m_nRectWidth = MyRect.right;
	Scale = m_nRectHeight / (pDoc->m_maxY - pDoc->m_minY);
	for (int i = 0; i < pDoc->m_nPointNum; i++)
	{
		m_OriginPoint[i].x = (pDoc->MyPoints[i].m_dLat - 414500) * Scale;
		m_OriginPoint[i].y = (pDoc->MyPoints[i].m_dLon - 4168900) * Scale;
		m_Point[i].x = m_OriginPoint[i].x;
		m_Point[i].y = m_OriginPoint[i].y;
	}
	Invalidate();
	// TODO: 在此添加命令处理程序代码
}


void CGISLineView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bButtonDown = true;
	switch (m_nFlag)
	{
	case 1:
	{
		SetCapture();
		m_pCentrePoint = point;
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		::SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		break;
	}
	case 2:
	{
		m_pStartPoint.x = point.x - m_nX;
		m_pStartPoint.y = -point.y+m_nY;
		break;
	}
	default:
		break;
	}
	
	CView::OnLButtonDown(nFlags, point);
}


void CGISLineView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDC* pDC = GetDC();
	CGISLineDoc* pDoc = GetDocument();
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (m_nFlag)
	{
	case 1:
	{
		::SetCursor(LoadCursor(NULL, IDC_HAND));
		m_nDeltaX = point.x - m_pCentrePoint.x;
		m_nDeltaY = point.y - m_pCentrePoint.y;
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		for (int i = 0; i < pDoc->m_nPointNum; i++)
		{
			m_Point[i].x = m_Point[i].x + m_nDeltaX;
			m_Point[i].y = m_Point[i].y - m_nDeltaY;
		}
		ReleaseCapture();
		break;
	}
	case 2:
	{
		double m_Scale;
		m_Scale = abs(m_pEndPoint.y - m_pStartPoint.y) / double(Height);
		Height*=m_Scale;
		Width *= m_Scale;
		break;
	}
	default:
		break;
	}
	m_bButtonDown = false;
	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}


void CGISLineView::OnMouseMove(UINT nFlags, CPoint point)
{

	if (m_nFlag == 2&&m_bButtonDown==true)
	{
		m_pEndPoint.x = point.x-m_nX;
		m_pEndPoint.y = -point.y+m_nY;
		Invalidate();
	}
	
	//CPoint m_EndPoint;
	//// TODO: 在此添加消息处理程序代码和/或调用默认值
	//if (m_bFlag == TRUE)
	//{
	//	m_EndPoint = point;
	//	m_nDeltaX=m_EndPoint.x- m_CentrePoint.x;
	//	m_nDeltaY = m_EndPoint.y - m_CentrePoint.y;
	//	for (int i = 0; i < 2467; i++)
	//	{
	//		m_Point[i].x = m_Point[i].x + m_nDeltaX;
	//		m_Point[i].y = m_Point[i].y - m_nDeltaY;
	//	}
	//	m_CentrePoint = m_EndPoint;
	//	Invalidate();
	//CDC* pDC = GetDC();
	////创建一个内存中的显示设备
	//CDC MemDC;
	//MemDC.CreateCompatibleDC(NULL);
	////创建一个内存中的图像
	//CBitmap MemBitmap;
	//CRect rect;
	//GetClientRect(&rect);
	//MemBitmap.CreateCompatibleBitmap(pDC, rect.right, rect.bottom);
	////指定内存显示设备在内存中的图像上画图
	//MemDC.SelectObject(&MemBitmap);
	////先用一种颜色作为内存显示设备的背景色
	//MemDC.FillSolidRect(rect.left, rect.top, rect.right, rect.bottom, RGB(144, 144, 144));
	//this->OnDraw(&MemDC);
	////将内存中画好的图像直接拷贝到屏幕指定区域上
	//pDC->BitBlt(rect.left, rect.top, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);

	////释放相关资源
	//ReleaseDC(pDC);
	//}
	CView::OnMouseMove(nFlags, point);
}


BOOL CGISLineView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if (zDelta >= 0)
	{
		m_Scale += zDelta % 100 / 100.0;
		Height = Height * m_Scale;
		Width = Width * m_Scale;
	}
	else
	{
		m_Scale -= abs(zDelta) % 100/100.0;
		Height= Height * m_Scale;
		Width = Width * m_Scale;
	}
	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CGISLineView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 在此添加专用代码和/或调用基类
		CRect rectClient;
		GetClientRect(rectClient);
		pDC->SetMapMode(MM_ANISOTROPIC);
		pDC->SetWindowExt(Height, Width);
		pDC->SetViewportExt(rectClient.right, -rectClient.bottom);
		pDC->SetViewportOrg(rectClient.right / 2, rectClient.bottom / 2);
		m_nX = rectClient.right / 2;
		m_nY = rectClient.bottom / 2;
	CView::OnPrepareDC(pDC, pInfo);
}


void CGISLineView::OnBigger()
{
	// TODO: 在此添加命令处理程序代码
}


void CGISLineView::On32782()
{
	// TODO: 在此添加命令处理程序代码
	m_pTestDlg = new CTestDlg();
	m_pTestDlg->Create(IDD_DIALOG1, this);
// 显示非模态对话框   
	m_pTestDlg->ShowWindow(SW_SHOW);
	UpdateData(FALSE);
}


void CGISLineView::OnBezierClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此处添加实现代码.
	
}

void CGISLineView::GetCtlPoint(CPoint point1, CPoint point2)
{
	
}

void CGISLineView::OnCal()
{
	// TODO: 在此添加命令处理程序代码
	//获取控制点
	CGISLineDoc* pDoc = GetDocument();
	CMyBezier b;
	double res;
	vector<Point> BezierPoint;
	vector<Point> CtlPoint;
	Point pt;
	for (int i = 0; i < pDoc->m_nPointNum; i++)
	{
		
		if ((pDoc->MyPoints[i].m_dLat != pDoc->MyPoints[i+1].m_dLat)|| (pDoc->MyPoints[i].m_dLon != pDoc->MyPoints[i+1].m_dLon))
		{
			pt = { pDoc->MyPoints[i].m_dLat, pDoc->MyPoints[i].m_dLon };
			BezierPoint.push_back(pt);
		}
		if (BezierPoint.size() >= 4)
		{
			double error = 1;
			CtlPoint = b.fit_curve(BezierPoint, error);
			double t = 0.0;
			vector<Point> ResPoints;
			vector<double> LineLength ;
			double max_length = 0;
			for (int j = 0; j < BezierPoint.size()-1; j++)
			{
				if (j == 0)
				{
					LineLength.push_back(0);
					
				}
				LineLength.push_back(sqrt((BezierPoint[j + 1].x - BezierPoint[j].x) * (BezierPoint[j + 1].x - BezierPoint[j].x) + (BezierPoint[j+ 1].y - BezierPoint[j].y) * (BezierPoint[j + 1].y - BezierPoint[j].y)));
				max_length += LineLength[j+1];
			}
			double length = 0;
			//计算偏差
			for (int i = 0; i < BezierPoint.size(); i++)
			{
				length += LineLength[i];
				Point a = b.at(CtlPoint, length / max_length);
				ResPoints.push_back(a);
			}
			double SumError = 0.0;
			for (int i = 0; i < BezierPoint.size(); i++)
			{
				SumError += (BezierPoint[i].x - ResPoints[i].x) * (BezierPoint[i].x - ResPoints[i].x) + (BezierPoint[i].y - ResPoints[i].y) * (BezierPoint[i].y - ResPoints[i].y);
			}
			res = sqrt(SumError) / BezierPoint.size();
			if (res >= 1.5)
			{
				Beziers.push_back(CtlPoint);
				BezierPoint.clear();
				i--;
				ResPoints.clear();
				CtlPoint.clear();
				LineLength.clear();
			}
		}
	}
	Invalidate();
}


void CGISLineView::OnOnbigger()
{
	// TODO: 在此添加命令处理程序代码
	double m_Scale = 0.9;
	Height = Height * m_Scale;
	Width = Width * m_Scale;
	Invalidate();
}


void CGISLineView::OnOnsmaller()
{
	// TODO: 在此添加命令处理程序代码
	double m_Scale = 2;
	Height = Height * m_Scale;
	Width = Width * m_Scale;
	Invalidate();
}


void CGISLineView::OnMove()
{
	// TODO: 在此添加命令处理程序代码
	m_nFlag = 1;
}


void CGISLineView::OnOnrectangle()
{
	// TODO: 在此添加命令处理程序代码
	m_nFlag = 2;
}


// 贝塞尔曲线生成
void CGISLineView::MyBezierCurve()
{
	// TODO: 在此处添加实现代码.
	CGISLineDoc* pDoc = GetDocument();
	CMyBezier b;
	double res;
	vector<Point> BezierPoint;
	vector<Point> CtlPoint;
	Point pt;
	for (int i = 0; i < pDoc->m_nPointNum; i++)
	{

		if ((pDoc->MyPoints[i].m_dLat != pDoc->MyPoints[i + 1].m_dLat) || (pDoc->MyPoints[i].m_dLon != pDoc->MyPoints[i + 1].m_dLon))
		{
			pt = { pDoc->MyPoints[i].m_dLat, pDoc->MyPoints[i].m_dLon };
			BezierPoint.push_back(pt);
		}
		if (BezierPoint.size() >= 4)
		{
			double error = 1;
			CtlPoint = b.fit_curve(BezierPoint, error);
			double t = 0.0;
			vector<Point> ResPoints;
			vector<double> LineLength;
			double max_length = 0;
			for (int j = 0; j < BezierPoint.size() - 1; j++)
			{
				if (j == 0)
				{
					LineLength.push_back(0);

				}
				LineLength.push_back(sqrt((BezierPoint[j + 1].x - BezierPoint[j].x) * (BezierPoint[j + 1].x - BezierPoint[j].x) + (BezierPoint[j + 1].y - BezierPoint[j].y) * (BezierPoint[j + 1].y - BezierPoint[j].y)));
				max_length += LineLength[j + 1];
			}
			double length = 0;
			//计算偏差
			for (int i = 0; i < BezierPoint.size(); i++)
			{
				length += LineLength[i];
				Point a = b.at(CtlPoint, length / max_length);
				ResPoints.push_back(a);
			}
			double SumError = 0.0;
			for (int i = 0; i < BezierPoint.size(); i++)
			{
				SumError += (BezierPoint[i].x - ResPoints[i].x) * (BezierPoint[i].x - ResPoints[i].x) + (BezierPoint[i].y - ResPoints[i].y) * (BezierPoint[i].y - ResPoints[i].y);
			}
			res = sqrt(SumError) / BezierPoint.size();

			if (res >= m_dMaxError)
			{
				m_dMaxError = res;
			}
			if (res >= m_dBezierThreshold)
			{
				m_dSumError += res;
				Beziers.push_back(CtlPoint);
				BezierPoint.clear();
				i--;
				ResPoints.clear();
				CtlPoint.clear();
				LineLength.clear();
			}
		}
	}
	Invalidate();
}
