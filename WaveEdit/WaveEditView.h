
// WaveEditView.h : interface of the CWaveEditView class

#pragma once
#include <stack>
#include "WaveFile.h"
#include "WaveEditDoc.h"
#include "WaveEditDoc.h"

class CWaveEditView : public CScrollView
{
	friend class WaveFile;
	friend class CWaveEditDoc;
	bool mousePressed;
	int selectionStart; //Selected sample start
	int selectionEnd; //Selected sample end
	double zoom; // amount of zoom
	double drawScale; //scale at which we draw the waves
	WaveFile* clipboard;

	std::stack<WaveFile> redoStack;
	std::stack<WaveFile> undoStack;

	// Delete a stack of WaveFile
	void deleteStack(std::stack<WaveFile> &stack);

protected: // create from serialization only
	
	DECLARE_DYNCREATE(CWaveEditView)

// Attributes
public:
	CWaveEditDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	CWaveEditView();
	virtual ~CWaveEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnEditCopy();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnFiltersSpeedup();
	afx_msg void OnFiltersSlowdown();
	afx_msg void OnFiltersEcho();
	afx_msg void OnFiltersReverse();
	afx_msg void OnSpeedup2x();
	afx_msg void OnSpeedup3x();
	afx_msg void OnSpeedup4x();
	afx_msg void OnSpeedup5x();
	afx_msg void OnSlowdownHalfx();
	afx_msg void OnSlowdownThirdx();
	afx_msg void OnSlowdownFourthx();
	afx_msg void OnSlowdownFifthx();
};

#ifndef _DEBUG  // debug version in WaveEditView.cpp
inline CWaveEditDoc* CWaveEditView::GetDocument() const
   { return reinterpret_cast<CWaveEditDoc*>(m_pDocument); }
#endif

