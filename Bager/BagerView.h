
// BagerView.h : interface of the CBagerView class
//

#pragma once
#include <corecrt_math_defines.h> 
#include "DImage.h"
#define RAD(x) M_PI*x/180.00


class CBagerView : public CView
{
protected: // create from serialization only
	CBagerView() noexcept;
	DECLARE_DYNCREATE(CBagerView)

// Attributes
public:
	CBagerDoc* GetDocument() const;
	DImage* pozadina = new DImage();
	DImage* bager = new DImage();
	DImage* arm1 = new DImage();
	DImage* arm2 = new DImage();
	HENHMETAFILE hViljuska;   // EMF handle
	CRect viljuskaFramePx;    // frame rectangle in pixels (easy to use for drawing)
	float bagerLeftRight = 0;
	float rotArm1 = -90;
	float rotArm2 = -90;
	float rotViljuska = -90;




// Operations
public:
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float dX, float dY, bool rightMultiply);
	void DrawExcavator(CDC* pDC);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawFork(CDC* pDC);
	void DrawBody(CDC* pDC);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CBagerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in BagerView.cpp
inline CBagerDoc* CBagerView::GetDocument() const
   { return reinterpret_cast<CBagerDoc*>(m_pDocument); }
#endif

