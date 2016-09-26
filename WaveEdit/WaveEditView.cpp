
// WaveEditView.cpp : implementation of the CWaveEditView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WaveEdit.h"
#endif

#include "WaveEditDoc.h"
#include "WaveEditView.h"
#include <sstream>
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveEditView

IMPLEMENT_DYNCREATE(CWaveEditView, CView)

BEGIN_MESSAGE_MAP(CWaveEditView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_CUT, &CWaveEditView::OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, &CWaveEditView::OnEditPaste)
	ON_COMMAND(ID_VIEW_ZOOMIN, &CWaveEditView::OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, &CWaveEditView::OnViewZoomout)
END_MESSAGE_MAP()

// CWaveEditView construction/destruction

CWaveEditView::CWaveEditView()
{
	// TODO: add construction code here
	mousePressed = false;
	selectionStart = 0;
	selectionEnd = 0;
	zoom = 1;

}

CWaveEditView::~CWaveEditView()
{
}

BOOL CWaveEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CWaveEditView drawing

void CWaveEditView::OnDraw(CDC* pDC)
{
	CWaveEditDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	WaveFile * wave = &pDoc->wave;

	if (wave->hdr == NULL) return;

	// Get dimensions of the window
	CRect rect;
	GetClientRect(rect);

	//Set color in pen and brush for the selection
	COLORREF penColor = RGB(255, 200, 200);
	CPen pen1(PS_SOLID, 0, penColor);
	pDC->SelectObject(&pen1);
	CBrush brush1(penColor);
	pDC->SelectObject(&brush1);

	// Draw selection if there is any
	if (selectionStart != selectionEnd)
		pDC->Rectangle(selectionStart, 0, selectionEnd, rect.Height());

	// Set pen and brush color for wave
	COLORREF color = RGB(0, 255, 0);
	CPen pen2(PS_SOLID, 0, color);
	pDC->SelectObject(&pen2);
	CBrush brush2(color);
	pDC->SelectObject(&brush2);

	// Draw the wave
	pDC->MoveTo(0, 0);
	int x;
	for (x = 0; x < zoom * wave->lastSample/drawScale; x++)
	{
		// assuming the whole file will be fit in the window, for every x value in the window
		// we need to find the equivalent sample in the wave file
		float val = wave->get_sample((int)(x*drawScale/zoom));
		// We need to fit the sound also in the y axis. The y axis goes from 0 in the
		//top of the window to rect.Height at the bottom. the sound goes from -32768 to 32767
		int y = (int)((val + 32768) * (rect.Height() - 1) / (32767 + 32768));
		pDC->LineTo(x, rect.Height() - y);
	}

	CSize sizeTotal;
	sizeTotal.cx = x;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

// Set the scroll extent

void CWaveEditView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// init scroll sizes
	CSize sizeTotal;
	SetScrollSizes(MM_TEXT, sizeTotal);

	// Set scaling
	CRect rect;
	GetClientRect(rect);
	CWaveEditDoc *pDoc = GetDocument();
	WaveFile wave = pDoc->wave;
	drawScale = wave.lastSample / rect.Width();

}

// CWaveEditView printing

BOOL CWaveEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWaveEditView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWaveEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CWaveEditView diagnostics

#ifdef _DEBUG
void CWaveEditView::AssertValid() const
{
	CView::AssertValid();
}

void CWaveEditView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWaveEditDoc* CWaveEditView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWaveEditDoc)));
	return (CWaveEditDoc*)m_pDocument;
}
#endif //_DEBUG


// CWaveEditView message handlers


void CWaveEditView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mousePressed = true;
	selectionStart = point.x;
	selectionEnd = point.x;
	CScrollView::OnLButtonDown(nFlags, point);
}


void CWaveEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mousePressed = false;
	this->selectionEnd = point.x;
	this->RedrawWindow();

	CScrollView::OnLButtonUp(nFlags, point);
}


void CWaveEditView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CScrollView::OnMouseMove(nFlags, point);

	if (mousePressed) {
		selectionEnd = point.x;
		RedrawWindow();
	}
}


void CWaveEditView::OnEditCut()
{
	// TODO: Add your command handler code here
	CWaveEditDoc* pDoc = GetDocument();

	OutputDebugStringW(L"My output string.");

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	WaveFile* wave = &pDoc->wave;

	if (wave->hdr == nullptr) return;

	//Get Dimensions of the current window
	CRect rect;
	GetClientRect(rect);

	//Scale the start section
	double start = (1000 * wave->lastSample / wave->sampleRate) * this->selectionStart / rect.Width();

	// Scale the end section
	double end = (1000 * wave->lastSample / wave->sampleRate) * this->selectionEnd / rect.Width();

	if (start > end) {
		double i = end;
		end = start;
		start = i;
	}

	//copy the first fragmet
	clipboard = wave->get_fragment(start, end);

	//copy the clipboard
	WaveFile* w2 = wave->remove_fragment(start, end);

	//Remove old wave
	//delete wave;

	// Substitute old wave with new one
	pDoc->wave = *w2;

	// Update window
	this->RedrawWindow();
}


void CWaveEditView::OnEditPaste()
{
	// TODO: Add your command handler code here
	CWaveEditDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	WaveFile* wave = &pDoc->wave;

	if (wave->hdr == nullptr) return;

	CRect rect;
	GetClientRect(rect);

	//Scale the start section
	double start = (1000 * wave->lastSample / wave->sampleRate) * this->selectionStart / rect.Width();

	if (clipboard != nullptr) {
		WaveFile* w2 = wave->insert_fragment(clipboard, start);
		pDoc->wave = *w2;
	}
	// Update window 
	this->RedrawWindow();
}

void CWaveEditView::OnViewZoomin()
{
	zoom = zoom * 2;
	this->RedrawWindow();
}


void CWaveEditView::OnViewZoomout()
{
	zoom = zoom / 2;
	this->RedrawWindow();
}
