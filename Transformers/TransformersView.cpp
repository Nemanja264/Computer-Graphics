
// TransformersView.cpp : implementation of the CTransformersView class
//

#include "pch.h"
#include "framework.h"
#include <algorithm>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Transformers.h"
#endif

#include "TransformersDoc.h"
#include "TransformersView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransformersView

IMPLEMENT_DYNCREATE(CTransformersView, CView)

BEGIN_MESSAGE_MAP(CTransformersView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CTransformersView construction/destruction

CTransformersView::CTransformersView() noexcept
{
	// TODO: add construction code here
	pozadina->Load(CString("./background.jpg"));
	leg1->Load(CString("./leg1.png"));
	leg2->Load(CString("./leg2.png"));
	arm1->Load(CString("./arm1.png"));
	arm2->Load(CString("./arm2.png"));
	body->Load(CString("./body1.png"));
}

CTransformersView::~CTransformersView()
{
	delete pozadina;
	delete arm1;
	delete arm2;
	delete leg1;
	delete leg2;
	delete body;
}

BOOL CTransformersView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CTransformersView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xform;
	xform.eM11 = 1;
	xform.eM21 = 0;
	xform.eM12 = 0;
	xform.eM22 = 1;
	xform.eDx = dX;
	xform.eDy = dY;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CTransformersView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float angle_rad = RAD(angle);

	XFORM xform;
	xform.eM11 = cos(angle_rad);
	xform.eM21 = -sin(angle_rad);
	xform.eM12 = sin(angle_rad);
	xform.eM22 = cos(angle_rad);
	xform.eDx = 0;
	xform.eDy = 0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CTransformersView::Scale(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xform;
	xform.eM11 = dX;
	xform.eM21 = 0;
	xform.eM12 = 0;
	xform.eM22 = dY;
	xform.eDx = 0;
	xform.eDy = 0;

	pDC->ModifyWorldTransform(&xform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CTransformersView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);

	int w = pImage->Width();
	int h = pImage->Height();
	CRect imgRect(0, 0, w, h);

	pImage->DrawTransparent(pDC, imgRect, imgRect, color);
}

void CTransformersView::DrawBackground(CDC* pDC, CRect rc)
{
	int w = pozadina->Width();
	int h = pozadina->Height();


	int rWidth = min(w, rc.Width());
	int rHeight = min(h, rc.Height());
	CPoint topLeft(rc.left, rc.top);
	CPoint rightBottom(rc.left + rWidth, rc.top + rHeight);

	CRect imgRect(0, 0, rWidth, rHeight);
	CRect dstRect(topLeft, rightBottom);

	pozadina->Draw(pDC, imgRect, dstRect);
}

void CTransformersView::DrawArm1(CDC* pDC)
{
	Translate(pDC, 34, 31, false);
	Rotate(pDC, -rotArm1, false);
	Translate(pDC, -34, -31, false);
	DrawImgTransparent(pDC, arm1);
}

void CTransformersView::DrawArm2(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);


	Translate(pDC, 178, 55, false);
	Translate(pDC, 34, 31, false);
	Rotate(pDC, -rotArm1, false);
	Translate(pDC, -34, -31, false);
	Translate(pDC, 187, 55, false);

	Translate(pDC, 23, 61, false);
	Rotate(pDC, 180-rotArm1, false);
	Translate(pDC, -23, -61, false);
	DrawImgTransparent(pDC, arm2);


	pDC->SetWorldTransform(&oldForm);
}

void CTransformersView::DrawLeg1(CDC* pDC)
{
	Translate(pDC, 30, 125, false);
	Rotate(pDC, -rotLeg1, false);
	Translate(pDC, -30, -125, false);
	DrawImgTransparent(pDC, leg1);
}

void CTransformersView::DrawLeg2(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	
	Translate(pDC, 35, 60, false);
	Rotate(pDC, rotLeg1, false);
	Translate(pDC, -35, -60, false);
	DrawImgTransparent(pDC, leg2);


	pDC->SetWorldTransform(&oldForm);
}

void CTransformersView::DrawBody(CDC* pDC)
{
	Translate(pDC, 26, 133, false);
	Rotate(pDC, rotBody, false);
	Translate(pDC, -26, -133, false);
	DrawImgTransparent(pDC, body);
}

void CTransformersView::DrawTransformer(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 200, 500, false);
	DrawLeg2(pDC);

	Translate(pDC, 5, -65, false);
	DrawLeg1(pDC);

	Translate(pDC, 237-23, -6, false);
	DrawBody(pDC);

	DrawArm2(pDC);

	Translate(pDC, 178, 55, false);
	DrawArm1(pDC);

	pDC->SetWorldTransform(&oldForm);
}
// CTransformersView drawing

void CTransformersView::OnDraw(CDC* pDC)
{
	CTransformersDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect clientRect;
	GetClientRect(&clientRect);
	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, clientRect.Width(), clientRect.Height());
	pMemDC->SelectObject(&bmp);

	int prevMode = pMemDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm;
	pMemDC->GetWorldTransform(&oldForm);

	DrawBackground(pMemDC, clientRect);
	DrawTransformer(pMemDC);


	pMemDC->SetWorldTransform(&oldForm);
	pMemDC->SetGraphicsMode(prevMode);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
	delete pMemDC;
	// TODO: add draw code for native data here
}


// CTransformersView printing

BOOL CTransformersView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTransformersView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTransformersView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTransformersView diagnostics

#ifdef _DEBUG
void CTransformersView::AssertValid() const
{
	CView::AssertValid();
}

void CTransformersView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTransformersDoc* CTransformersView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTransformersDoc)));
	return (CTransformersDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CTransformersView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTransformersView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT)
	{	
		if(rotLeg1 < 105)
			rotLeg1 += 5;

		if (rotBody < 15)
			rotBody += 1;

		if (rotArm1 < 270)
			rotArm1 += 13;
	}
	else if (nChar == VK_RIGHT)
	{
		if (rotLeg1 >= 5)
			rotLeg1 -= 5;

		if (rotBody >= 1)
			rotBody -= 1;

		if (rotArm1 >= 13)
			rotArm1 -= 13;
	}
	else if (nChar == 'W')
	{
		if (rotBody < 15)
			rotBody += 5;
	}
	else if (nChar == 'S')
	{
		if (rotBody >= 5)
			rotBody -= 5;
	}
	else if (nChar == 'Q')
	{
		if (rotLeg1 < 105)
			rotLeg1 += 5;

		if (rotArm1 < 270)
			rotArm1 += 13;
	}
	else if (nChar == 'A')
	{
		if (rotLeg1 >= 5)
			rotLeg1 -= 5;

		if (rotArm1 >= 13)
			rotArm1 -= 13;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
// CTransformersView message handlers
