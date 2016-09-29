
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

#include "FilterEcho.h"
#include "FilterReverse.h"
#include "FilterSlowdown.h"
#include "FilterSpeedup.h"
#include "WaveEditView.h"

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
	ON_COMMAND(ID_TOOLS_REVERSE, &CWaveEditDoc::OnToolsReverse)
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
	WaveFile* speed = new WaveFile();
	//speed = wave.speedUp(2);
	speed = FilterSpeedup::transform(2, &wave);
	wave = *speed;
	wave.play();
}


void CWaveEditDoc::OnToolsSlowdown()
{
	WaveFile* slow = new WaveFile();
	//slow = wave.slowDown(1.8);
	slow = FilterSlowdown::transform(1.7, &wave);
	wave = *slow;
	wave.play();
}


void CWaveEditDoc::OnToolsEcho()
{
	// Right now have a copy of both the original and the new echo version
	WaveFile* echo = new WaveFile();
	//echo = wave.echo(.7, 15);
	echo = FilterEcho::transform(.7, 15, &wave);
	wave = *echo;
	wave.play();
}


void CWaveEditDoc::OnToolsReverse()
{
	// TODO: Add your command handler code here
	WaveFile* reverse = new WaveFile();
	//reverse = wave.reverse();
	reverse = FilterReverse::transform(0, &wave);
	wave = *reverse;
	wave.play();
}
