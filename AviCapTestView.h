// AviCapTestView.h : interface of the CAviCapTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVICAPTESTVIEW_H__5D2A240E_891F_11D2_B4B8_444553540000__INCLUDED_)
#define AFX_AVICAPTESTVIEW_H__5D2A240E_891F_11D2_B4B8_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "cavicap.h"
#include "SudBitmap.h"

class CAviCapTestView : public CView
{
protected: // create from serialization only
	CAviCapTestView();
	DECLARE_DYNCREATE(CAviCapTestView)
	LARGE_INTEGER m_nFreq;
	LARGE_INTEGER m_nBeginTime;
	Bin m_strongestLine;
	DisplaySud m_display;

// Attributes
public:
	CAviCapTestDoc* GetDocument();
	CAviCap		CaptureWindow;
	BOOL		bEnableProcessing;
// Operations
public:
	void	DoSomeImageProcessing(LPVIDEOHDR lpVHdr);
	void Tic();
	double Toc();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAviCapTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAviCapTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAviCapTestView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnFileDisplay();
	afx_msg void OnFileDriver();
	afx_msg void OnFileFormat();
	afx_msg void OnFilePreview();
	afx_msg void OnUpdateFilePreview(CCmdUI* pCmdUI);
	afx_msg void OnFileSource();
	afx_msg void OnUpdateFileSource(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileFormat(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileDriver(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileDisplay(CCmdUI* pCmdUI);
	afx_msg void OnViewAutosize();
	afx_msg void OnUpdateViewAutosize(CCmdUI* pCmdUI);
	afx_msg void OnFileenhctl();
	afx_msg void OnUpdateFileenhctl(CCmdUI* pCmdUI);
	afx_msg void OnViewMinus();
	afx_msg void OnViewPlus();
	afx_msg void OnUpdateViewMinus(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPlus(CCmdUI* pCmdUI);
	afx_msg void OnViewProcess();
	afx_msg void OnUpdateViewProcess(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewBlack();
	afx_msg void OnUpdateViewBlack(CCmdUI *pCmdUI);
	afx_msg void OnViewWhite();
	afx_msg void OnUpdateViewWhite(CCmdUI *pCmdUI);
	afx_msg void OnViewRotationdetection();
	afx_msg void OnUpdateViewRotationdetection(CCmdUI *pCmdUI);
	afx_msg void OnViewWhitearea();
	afx_msg void OnUpdateViewWhitearea(CCmdUI *pCmdUI);
	afx_msg void OnViewGrid();
	afx_msg void OnUpdateViewGrid(CCmdUI *pCmdUI);
	afx_msg void OnViewOcrresult();
	afx_msg void OnUpdateViewOcrresult(CCmdUI *pCmdUI);
	afx_msg void OnViewLinesdetect();
	afx_msg void OnUpdateViewLinesdetect(CCmdUI *pCmdUI);
	afx_msg void OnViewWhiterect();
	afx_msg void OnUpdateViewWhiterect(CCmdUI *pCmdUI);
	afx_msg void OnViewOcraveraged();
	afx_msg void OnUpdateViewOcraveraged(CCmdUI *pCmdUI);
	afx_msg void OnViewSolution();
	afx_msg void OnUpdateViewSolution(CCmdUI *pCmdUI);
	afx_msg void OnViewFinalSolution();
	afx_msg void OnUpdateViewFinalSolution(CCmdUI *pCmdUI);
	afx_msg void OnViewSmallfont();
	afx_msg void OnUpdateViewSmallfont(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in AviCapTestView.cpp
inline CAviCapTestDoc* CAviCapTestView::GetDocument()
   { return (CAviCapTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVICAPTESTVIEW_H__5D2A240E_891F_11D2_B4B8_444553540000__INCLUDED_)
