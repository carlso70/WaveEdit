
// WaveEditDoc.cpp : implementation of the CWaveEditDoc class
//

#include "stdafx.h"
#include <iostream>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaveEdit.h"
#endif

#include "WaveEditDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWaveEditDoc

IMPLEMENT_DYNCREATE(CWaveEditDoc, CDocument)

BEGIN_MESSAGE_MAP(CWaveEditDoc, CDocument)
	ON_COMMAND(ID_TOOLS_PLAY, &CWaveEditDoc::OnToolsPlay)
	ON_COMMAND(ID_TOOLS_SPEEDUP, &CWaveEditDoc::OnToolsSpeedup)
	ON_COMMAND(ID_TOOLS_SLOWDOWN, &CWaveEditDoc::OnToolsSlowdown)
	ON_COMMAND(ID_TOOLS_ECHO, &CWaveEditDoc::OnToolsEcho)
END_MESSAGE_MAP()


// CWaveEditDoc construction/destruction

CWaveEditDoc::CWaveEditDoc()
{
	// TODO: add one-time construction code here

}

CWaveEditDoc::~CWaveEditDoc()
{
}

BOOL CWaveEditDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CWaveEditDoc serialization
// Ran when you use the "Open" or "Save As" or "Save" options
void CWaveEditDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		wave.read(ar.GetFile());
		wave.play();
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWaveEditDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
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

// Support for Search Handlers
void CWaveEditDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWaveEditDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWaveEditDoc diagnostics

#ifdef _DEBUG
void CWaveEditDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWaveEditDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWaveEditDoc commands




void CWaveEditDoc::OnToolsPlay()
{
	// TODO: Add your command handler code here
	wave.play();
}


void CWaveEditDoc::OnToolsSpeedup()
{
	std::cout << "Speed Up" << std::endl;

	WaveFile* speed = new WaveFile();
	speed = wave.speedUp(1);
	speed->play();

}


void CWaveEditDoc::OnToolsSlowdown()
{
	std::cout << "Slow Down" << std::endl;
	
	WaveFile* slow = new WaveFile();
	slow = wave.slowDown(2);
	slow->play();

}


void CWaveEditDoc::OnToolsEcho()
{
	std::cout << "ECHO ECHO ECHO";

	// Right now have a copy of both the original and the new echo version
	WaveFile* echo = new WaveFile();
	echo = wave.echo(.7, 15);
	echo->play();

}
