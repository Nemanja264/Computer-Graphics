
// TransformersView.h : interface of the CTransformersView class
//

#pragma once
#include "DImage.h"
#include <corecrt_math_defines.h>
#define RAD(x) M_PI*x/180.00

class CTransformersView : public CView
{
protected: // create from serialization only
	CTransformersView() noexcept;
	DECLARE_DYNCREATE(CTransformersView)

// Attributes
public:
	CTransformersDoc* GetDocument() const;
	DImage* leg2 = new DImage();
	DImage* leg1 = new DImage();
	DImage* arm2 = new DImage();
	DImage* arm1 = new DImage();
	DImage* body = new DImage();
	DImage* pozadina = new DImage();
	float rotArm1 = 0;
	float rotBody = 0;
	float rotLeg1 = 0;
	float rotLeg2 = 0;




// Operations
public:
	void DrawBackground(CDC* pDC, CRect rc);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float dX, float dY, bool rightMultiply);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);
	void DrawBody(CDC* pDC);
	void DrawTransformer(CDC* pDC);
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
	virtual ~CTransformersView();
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

#ifndef _DEBUG  // debug version in TransformersView.cpp
inline CTransformersDoc* CTransformersView::GetDocument() const
   { return reinterpret_cast<CTransformersDoc*>(m_pDocument); }
#endif

