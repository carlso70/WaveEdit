
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
#include "FilterReverse.h"
#include "FilterEcho.h"
#include "FilterSlowdown.h"
#include "FilterSpeedup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWaveEditView

IMPLEMENT_DYNCREATE(CWaveEditView, CScrollView)

BEGIN_MESSAGE_MAP(CWaveEditView, CScrollView)
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
	ON_COMMAND(ID_EDIT_COPY, &CWaveEditView::OnEditCopy)
	ON_COMMAND(ID_EDIT_UNDO, &CWaveEditView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CWaveEditView::OnEditRedo)
	ON_COMMAND(ID_FILTERS_SPEEDUP, &CWaveEditView::OnFiltersSpeedup)
	ON_COMMAND(ID_FILTERS_SLOWDOWN, &CWaveEditView::OnFiltersSlowdown)
	ON_COMMAND(ID_FILTERS_ECHO, &CWaveEditView::OnFiltersEcho)
	ON_COMMAND(ID_FILTERS_REVERSE, &CWaveEditView::OnFiltersReverse)
END_MESSAGE_MAP()

// CWaveEditView construction/destruction

CWaveEditView::CWaveEditView()
{
	mousePressed = false;
	selectionStart = 0;
	selectionEnd = 0;
	zoom = 1;
}

CWaveEditView::~CWaveEditView()
{
	deleteStack(undoStack);
	deleteStack(redoStack);
	delete clipboard;
}

void CWaveEditView::deleteStack(std::stack<WaveFile> &stack) {
	while (stack.empty() != true) {
		stack.pop();
	}
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
	for (x = 0; x < zoom * wave->lastSample / drawScale; x++)
	{
		// assuming the whole file will be fit in the window, for every x value in the window
		// we need to find the equivalent sample in the wave file
		float val = wave->get_sample((int)(x*drawScale / zoom));
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
	sizeTotal.cx = 10000;
	sizeTotal.cy = 10000;
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
	CPoint scrollPos = GetDeviceScrollPosition();
	selectionStart = point.x + scrollPos.x;
	selectionEnd = point.x + scrollPos.x;

	CScrollView::OnLButtonDown(nFlags, point);
}


void CWaveEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	mousePressed = false;
	CPoint scrollPos = GetDeviceScrollPosition();
	this->selectionEnd = point.x + scrollPos.x;
	if (selectionEnd < selectionStart) {
		int i = selectionStart;
		selectionStart = selectionEnd;
		selectionEnd = i;
	}
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
	CWaveEditDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)	return;
	WaveFile* wave = &pDoc->wave;

	if (wave->hdr == nullptr) return;

	// Undo operations, add the current document wave to the undo stack
	undoStack.push(*wave);
	deleteStack(redoStack);

	//Get Dimensions of the current window
	CRect rect;
	GetClientRect(rect);

	// We want the totalSize of the wave in the window
	CSize totalSize = GetTotalSize();

	//Scale the start section
	double start = (1000 * wave->lastSample / wave->sampleRate) * this->selectionStart / (double)totalSize.cx;

	// Scale the end section
	double end = (1000 * wave->lastSample / wave->sampleRate) * this->selectionEnd / (double)totalSize.cx;

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


void CWaveEditView::OnEditCopy()
{
	CWaveEditDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	WaveFile* wave = &pDoc->wave;


	// Undo operations
	undoStack.push(*wave);
	deleteStack(redoStack);


	//Get Dimensions of the current window
	CRect rect;
	GetClientRect(rect);

	// We want the totalSize of the wave in the window
	CSize totalSize = GetTotalSize();

	//Scale the start section
	double start = (1000 * wave->lastSample / wave->sampleRate) * this->selectionStart / (double)totalSize.cx;

	// Scale the end section
	double end = (1000 * wave->lastSample / wave->sampleRate) * this->selectionEnd / (double)totalSize.cx;

	if (start > end) {
		double i = end;
		end = start;
		start = i;
	}

	// Copy the fragment onto the clipboard
	clipboard = wave->get_fragment(start, end);

	this->RedrawWindow();
}



void CWaveEditView::OnEditPaste()
{
	CWaveEditDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	WaveFile* wave = &pDoc->wave;

	if (wave->hdr == nullptr) return;

	// Undo operations
	undoStack.push(*wave);
	deleteStack(redoStack);


	CRect rect;
	GetClientRect(rect);

	CSize totalSize = GetTotalSize();

	//Scale the start section
	double start = (1000 * wave->lastSample / wave->sampleRate) * this->selectionStart / (double)totalSize.cx;

	if (clipboard != nullptr) {
		WaveFile* w2 = wave->insert_fragment(clipboard, start);

		// Set the new copied wave to the document wave
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

void CWaveEditView::OnEditUndo()
{
	if (undoStack.empty())
		return;
	CWaveEditDoc* pDoc = GetDocument();
	redoStack.push(pDoc->wave);
	pDoc->wave = undoStack.top();
	pDoc->wave.updateHeader();
	undoStack.pop();
	this->RedrawWindow();
}


void CWaveEditView::OnEditRedo()
{
	if (redoStack.empty())
		return;
	CWaveEditDoc* pDoc = GetDocument();
	undoStack.push(pDoc->wave);
	pDoc->wave = redoStack.top();
	pDoc->wave.updateHeader();
	redoStack.pop();
	this->RedrawWindow();
}



void CWaveEditView::OnFiltersSpeedup()
{
	CWaveEditDoc* pDoc = GetDocument();
	WaveFile* speed = new WaveFile();

	undoStack.push(pDoc->wave);
	deleteStack(redoStack);

	speed = FilterSpeedup::transform(2, &pDoc->wave);

	pDoc->wave = *speed;
	pDoc->wave.play();
}


void CWaveEditView::OnFiltersSlowdown()
{
	CWaveEditDoc* pDoc = GetDocument();
	WaveFile* slow = new WaveFile();

	undoStack.push(pDoc->wave);
	deleteStack(redoStack);

	slow = FilterSlowdown::transform(.5, &pDoc->wave);

	pDoc->wave = *slow;
	pDoc->wave.play();

}


void CWaveEditView::OnFiltersEcho()
{
	CWaveEditDoc* pDoc = GetDocument();
	WaveFile * echo = new WaveFile();

	undoStack.push(pDoc->wave);
	deleteStack(redoStack);

	echo = FilterEcho::transform(.7, 15, &pDoc->wave);

	pDoc->wave = *echo;
	pDoc->wave.play();
}


void CWaveEditView::OnFiltersReverse()
{
	CWaveEditDoc* pDoc = GetDocument();
	WaveFile* reverse = new WaveFile();

	undoStack.push(pDoc->wave);
	deleteStack(redoStack);

	reverse = FilterReverse::transform(0, &pDoc->wave);

	pDoc->wave = *reverse;
	pDoc->wave.play();
}
