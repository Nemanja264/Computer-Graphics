
// RobotView.h : interface of the CRobotView class
//
#include <corecrt_math_defines.h>
#define RAD(x) M_PI*x/180.00
#include "DImage.h"

#pragma once


class CRobotView : public CView
{
protected: // create from serialization only
	CRobotView() noexcept;
	DECLARE_DYNCREATE(CRobotView)

// Attributes
public:
	CRobotDoc* GetDocument() const;
	DImage* pozadina = new DImage();
	DImage* glava = new DImage();
	DImage* nadkolenica = new DImage();
	DImage* nadlaktica = new DImage();
	DImage* podkolenica = new DImage();
	DImage* podlaktica = new DImage();
	DImage* saka = new DImage();
	DImage* stopalo = new DImage();
	DImage* telo = new DImage();

	float rotNadLaktica = 0;
	float rotPodLaktica = 0;
	float rotSaka = 0;
	float ceo = 0;
	float skaliranje = 1.0;

// Operations
public:
	void Translate(CDC* pDC, float dx, float dy, bool rightMultiply);
	void Scale(CDC* pDC, float dx, float dy, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawBackground(CDC* pDC);
	void DrawRobot(CDC* pDC);
	void DrawHalf(CDC* pDC);


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
	virtual ~CRobotView();
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

#ifndef _DEBUG  // debug version in RobotView.cpp
inline CRobotDoc* CRobotView::GetDocument() const
   { return reinterpret_cast<CRobotDoc*>(m_pDocument); }
#endif

