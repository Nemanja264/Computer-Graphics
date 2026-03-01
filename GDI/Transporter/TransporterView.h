
// TransporterView.h : interface of the CTransporterView class
//

#pragma once
#include "DImage.h"
#include <corecrt_math_defines.h>
#define RAD(x) M_PI*x/180.00

class CTransporterView : public CView
{
protected: // create from serialization only
	CTransporterView() noexcept;
	DECLARE_DYNCREATE(CTransporterView)

// Attributes
public:
	CTransporterDoc* GetDocument() const;
	DImage* pozadina = new DImage();
	DImage* body = new DImage();
	DImage* legUp = new DImage();
	DImage* legMiddle = new DImage();
	DImage* foot = new DImage();

	float z1 = 20;
	int idx = 0;
	int pidx = 2;
	int dir = 1;
	float scaled = 1.0;
	int pdir = 1;
	float a[5] = { -20, -10, 0, 10, 20 };
	float p1 = 0;
	float BGX = 0;
	float BGY = 0;
// Operations
public:
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float dX, float dY, bool rightMultiply);
	void LoadIdentity(CDC* pDC);
	void DrawScene(CDC* pDC, CRect rect);
	void DrawLeg(CDC* pDC, double alpha, double dx, double dy);
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
	virtual ~CTransporterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in TransporterView.cpp
inline CTransporterDoc* CTransporterView::GetDocument() const
   { return reinterpret_cast<CTransporterDoc*>(m_pDocument); }
#endif

