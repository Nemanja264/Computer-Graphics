
// Vezba1View.cpp : implementation of the CVezba1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Vezba1.h"
#endif

#include "Vezba1Doc.h"
#include "Vezba1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DImage.h"

// CVezba1View

IMPLEMENT_DYNCREATE(CVezba1View, CView)

BEGIN_MESSAGE_MAP(CVezba1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CVezba1View construction/destruction

CVezba1View::CVezba1View() noexcept
{
	// TODO: add construction code here
	base->Load(CString("D:/Fax/7. semestar/Racunarska Grafika/KOL_1/lampa/base.png"));
	arm1->Load(CString("D:/Fax/7. semestar/Racunarska Grafika/KOL_1/lampa/arm1.png"));
	arm2->Load(CString("D:/Fax/7. semestar/Racunarska Grafika/KOL_1/lampa/arm2.png"));
	head->Load(CString("D:/Fax/7. semestar/Racunarska Grafika/KOL_1/lampa/head.png"));
	pozadina->Load(CString("D:/Fax/7. semestar/Racunarska Grafika/KOL_1/lampa/pozadina.jpg"));
	base_shadow->Load(CString("D:/Fax/7. semestar/Racunarska Grafika/KOL_1/lampa/base_shadow.png"));

	arm1_shadow->Load(CString("D:/Fax/7. semestar/Racunarska Grafika/KOL_1/lampa/arm1_shadow.png"));

	arm2_shadow->Load(CString("D:/Fax/7. semestar/Racunarska Grafika/KOL_1/lampa/arm2_shadow.png"));
	head_shadow->Load(CString("D:/Fax/7. semestar/Racunarska Grafika/KOL_1/lampa/head_shadow.png"));


}

CVezba1View::~CVezba1View()
{
	delete base;
	delete arm1;
	delete arm2;
	delete head;
	delete pozadina;
}

BOOL CVezba1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
void CVezba1View::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
{
	XFORM xfrom;
	xfrom.eM11 = 1;
	xfrom.eM12 = 0;
	xfrom.eM22 = 1;
	xfrom.eM21 = 0;
	xfrom.eDx = dx;
	xfrom.eDy = dy;

	pDC->ModifyWorldTransform(&xfrom, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CVezba1View::Scale(CDC* pDC, float dx, float dy, bool rightMultiply)
{
	XFORM xfrom;
	xfrom.eM11 = dx;
	xfrom.eM12 = 0;
	xfrom.eM22 = dy;
	xfrom.eM21 = 0;
	xfrom.eDx = 0;
	xfrom.eDy = 0;

	pDC->ModifyWorldTransform(&xfrom, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CVezba1View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float angle_rad = RAD(angle);

	XFORM xfrom;
	xfrom.eM11 = cos(angle_rad);
	xfrom.eM12 = sin(angle_rad);
	xfrom.eM22 = cos(angle_rad);
	xfrom.eM21 = -sin(angle_rad);
	xfrom.eDx = 0;
	xfrom.eDy = 0;

	pDC->ModifyWorldTransform(&xfrom, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CVezba1View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);

	int w = pImage->Width();
	int h = pImage->Height();

	CRect rect(0, 0, w, h);
	pImage->DrawTransparent(pDC, rect, rect, color);
}

void CVezba1View::DrawBackground(CDC* pDC) {
	int w = pozadina->Width();
	int h = pozadina->Height();

	CRect imgRect(0, 0, w, h);

	CRect clRect;
	GetClientRect(clRect);

	CPoint topLeft(clRect.Width() / 2 - w / 2, clRect.Height() - h);
	CPoint bottomRight(clRect.Width() / 2 + w / 2, clRect.Height());

	pozadina->Draw(pDC, imgRect, CRect(topLeft, bottomRight));
}

void CVezba1View::DrawLamp(CDC* pDC, bool bIsShadow)
{
	XFORM xformOld;
	pDC->GetWorldTransform(&xformOld);


	DrawLampBase(pDC, bIsShadow);
	DrawLampArm1(pDC, bIsShadow);
	DrawLampHead(pDC, bIsShadow);
	DrawLampArm2(pDC, bIsShadow);

	pDC->SetWorldTransform(&xformOld);

}

void CVezba1View::DrawLampBase(CDC* pDC, bool bIsShadow)
{
	DrawImgTransparent(pDC, bIsShadow ? base_shadow : base);
}

void CVezba1View::DrawLampArm1(CDC* pDC, bool bIsShadow)
{
	Translate(pDC, 158, 10, false);
	Rotate(pDC, arm1angle, false);
	Translate(pDC, -58, -61, false);

	DrawImgTransparent(pDC, bIsShadow ? arm1_shadow : arm1);
}

void CVezba1View::DrawLampArm2(CDC* pDC, bool bIsShadow)
{

	Translate(pDC, 309, 61, false);
	Rotate(pDC, arm2angle, false);
	Translate(pDC, -36,-40, false);


	DrawImgTransparent(pDC, bIsShadow ? arm2_shadow : arm2);

}


void CVezba1View::DrawLampHead(CDC* pDC, bool bIsShadow)
{
	XFORM xformOld;
	pDC->GetWorldTransform(&xformOld);


	Translate(pDC, 309, 61, false);
	Rotate(pDC, arm2angle, false);
	Translate(pDC, -36, -40, false);

	Translate(pDC, 272, 40, false);
	Rotate(pDC, -190, false);
	Translate(pDC, -178, -100, false);

	DrawImgTransparent(pDC, bIsShadow ? head_shadow : head);

	pDC->SetWorldTransform(&xformOld);

}


void CVezba1View::DrawLampShadow(CDC* pDC)
{
	XFORM xformOld;
	pDC->GetWorldTransform(&xformOld);

	Scale(pDC, 1, 0.25, false);
	Rotate(pDC, -90, false);
	DrawLamp(pDC, true);

	pDC->SetWorldTransform(&xformOld);

}
// CVezba1View drawing

void CVezba1View::OnDraw(CDC* pDC)
{
	CVezba1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);
	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pMemDC->SelectObject(&bmp);

	int oldMode = pMemDC->SetGraphicsMode(GM_ADVANCED);
	XFORM oldForm;
	pMemDC->GetWorldTransform(&oldForm);

	DrawBackground(pMemDC);

	Translate(pMemDC, rect.Width() / 2, rect.Height() - 200, false);
	DrawLampShadow(pMemDC);

	Translate(pMemDC, 0, -100, false);
	DrawLamp(pMemDC, false);

	pMemDC->SetWorldTransform(&oldForm);
	pMemDC->SetGraphicsMode(oldMode);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
	delete pMemDC;

	// TODO: add draw code for native data here
}


// CVezba1View printing

BOOL CVezba1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVezba1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVezba1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CVezba1View diagnostics

#ifdef _DEBUG
void CVezba1View::AssertValid() const
{
	CView::AssertValid();
}

void CVezba1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVezba1Doc* CVezba1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVezba1Doc)));
	return (CVezba1Doc*)m_pDocument;
}


#endif //_DEBUG


BOOL CVezba1View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CVezba1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'A' || nChar == 'a')
		arm1angle += 5;
	if (nChar == 'D' || nChar == 'd')
		arm1angle -= 5;

	if (nChar == 'Q' || nChar == 'q')
		arm2angle += 5;
	if (nChar == 'E' || nChar == 'e')
		arm2angle -= 5;

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
// CVezba1View message handlers
