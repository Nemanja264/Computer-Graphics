
// Vezba1View.h : interface of the CVezba1View class
//
#include <corecrt_math_defines.h>
#pragma once
#include "DImage.h"
#define RAD(x) x* M_PI/180.00

class CVezba1View : public CView
{
protected: // create from serialization only
	CVezba1View() noexcept;
	DECLARE_DYNCREATE(CVezba1View)

// Attributes
public:
	CVezba1Doc* GetDocument() const;
	DImage* arm1 = new DImage();
	DImage* arm2 = new DImage();
	DImage* base = new DImage();
	DImage* head = new DImage();
	DImage* pozadina = new DImage();
	DImage* base_shadow = new DImage();
	DImage* arm1_shadow = new DImage();
	DImage* arm2_shadow = new DImage();
	DImage* head_shadow = new DImage();

	float arm1angle = -50;

	float arm2angle = -80;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void Translate(CDC* pDC, float dx, float dy, bool rightMultiply);
	void Scale(CDC* pDC, float dx, float dy, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBackground(CDC* pDC);
	void DrawLamp(CDC* pDC, bool bIsShadow);
	void DrawLampBase(CDC* pDC, bool bIsShadow);
	void DrawLampArm1(CDC* pDC, bool bIsShadow);
	void DrawLampArm2(CDC* pDC, bool bIsShadow);
	void DrawLampHead(CDC* pDC, bool bIsShadow);
	void DrawLampShadow(CDC* pDC);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CVezba1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in Vezba1View.cpp
inline CVezba1Doc* CVezba1View::GetDocument() const
   { return reinterpret_cast<CVezba1Doc*>(m_pDocument); }
#endif

