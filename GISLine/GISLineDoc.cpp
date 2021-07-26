
// GISLineDoc.cpp: CGISLineDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "GISLine.h"
#endif

#include "GISLineDoc.h"


#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGISLineDoc

IMPLEMENT_DYNCREATE(CGISLineDoc, CDocument)

BEGIN_MESSAGE_MAP(CGISLineDoc, CDocument)
	ON_COMMAND(ID_OPENFILE, &CGISLineDoc::OnOpenfile)
END_MESSAGE_MAP()


// CGISLineDoc 构造/析构

CGISLineDoc::CGISLineDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	m_nPointNum = 0;
	m_nLineNum = 0;
}

CGISLineDoc::~CGISLineDoc()
{
}

BOOL CGISLineDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CGISLineDoc 序列化

void CGISLineDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CGISLineDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CGISLineDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CGISLineDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGISLineDoc 诊断

#ifdef _DEBUG
void CGISLineDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGISLineDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGISLineDoc 命令


void CGISLineDoc::OnOpenfile()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, _T("文本文档|*.txt|"), NULL);
	dlg.m_ofn.lpstrInitialDir = _T("C:\\Users\\Administrator\\Desktop");//初始化目录
	if (dlg.DoModal() == IDOK)//==IDOK代表按下了打开按钮
		m_sFilePath =dlg.GetPathName();//获取文件的路径
	this->GetTxt();
}




void CGISLineDoc::GetTxt()
{
	// TODO: 在此处添加实现代码.
	// TODO: 在此处添加实现代码.
	try
	{
		CStdioFile file;
		file.Open(m_sFilePath, CFile::modeRead);
		CString MyStr, strLine, csTemp;
		file.ReadString(strLine);
		while (!strLine.IsEmpty())
		{
			int strNum = 4;
			for (int j = 0; j < strNum; j++)
			{
				AfxExtractSubString(csTemp, strLine, j, ' ');
				if (csTemp == csTemp.SpanIncluding(_T("0123456789.")))
				{
					PointData[m_nPointNum][j] = _tstof(csTemp);
				}
				else
				{
					continue;
				}
			}
			file.ReadString(strLine);
			m_nPointNum += 1;
		}
	}
	catch (CFileException* e)
	{
		e->ReportError();
		e->Delete();
	}
	//点坐标和ID存入MyPoint
	for (int i = 0; i < m_nPointNum; i++)
	{
		MyPoints[i].m_dLat = PointData[i][0];
		MyPoints[i].m_dLon = PointData[i][1];
		if (m_maxX <= PointData[i][0])
		{
			m_maxX = PointData[i][0];
		}
		if (m_maxY <= PointData[i][1])
		{
			m_maxY = PointData[i][1];
		}
		if (PointData[i][1] <= m_minY)
		{
			m_minY = PointData[i][1];
		}
		if (PointData[i][0] <= m_minX)
		{
			m_minX = PointData[i][0];
		}
		MyPoints[i].m_nID = i;
		LineID = 1;
	}
	m_nLineNum = 1;
}


void CGISLineDoc::GetControlPoint()
{
	// TODO: 在此处添加实现代码.

}
