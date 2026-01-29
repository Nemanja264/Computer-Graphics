
// TransporterView.cpp : implementation of the CTransporterView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Transporter.h"
#endif

#include "TransporterDoc.h"
#include "TransporterView.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransporterView

IMPLEMENT_DYNCREATE(CTransporterView, CView)

BEGIN_MESSAGE_MAP(CTransporterView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CTransporterView construction/destruction

CTransporterView::CTransporterView() noexcept
{
	// TODO: add construction code here
	pozadina->Load(CString("./Back2.jpg"));
	body->Load(CString("./Body.png"));
	legUp->Load(CString("./Leg1.png"));
	legMiddle->Load(CString("./Leg2.png"));
	foot->Load(CString("./Leg3.png"));

}

CTransporterView::~CTransporterView()
{
	delete pozadina;
	delete body;
	delete legUp;
	delete legMiddle;
	delete foot;


}

BOOL CTransporterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
void CTransporterView::DrawBackground(CDC* pDC)
{

	float w = pozadina->Width();
	float h = pozadina->Height();

	CRect clientRect;
	GetClientRect(&clientRect);

	CRect imgRect(BGX, BGY, BGX + clientRect.Width(), BGY + clientRect.Height());
	CRect dstRect(0,0, clientRect.Width(), clientRect.Height());
	

	pozadina->Draw(pDC, imgRect, dstRect);
}

void CTransporterView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);


	float w = pImage->Width();
	float h = pImage->Height();
	CRect imgRect(0, 0, w, h);

	pImage->DrawTransparent(pDC, imgRect, imgRect, color);
}

void CTransporterView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CTransporterView::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void CTransporterView::Scale(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CTransporterView::LoadIdentity(CDC* pDC)
{
	pDC->ModifyWorldTransform(nullptr, MWT_IDENTITY);

}

void CTransporterView::DrawScene(CDC* pDC, CRect rect)
{
	LoadIdentity(pDC);
	DrawBackground(pDC);

	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 100, 100, false);
	Scale(pDC, scaled, scaled, false);
	DrawImgTransparent(pDC, body);


	//zadnje noge

	DrawLeg(pDC, -z1, 72, 168);
	DrawLeg(pDC, z1, 72, 168);

	DrawLeg(pDC, -p1, 270, 168);
	DrawLeg(pDC, p1, 270, 168);



	pDC->SetWorldTransform(&oldForm);
}

void CTransporterView::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, dx, dy, false);
	Rotate(pDC, alpha, false);
	Translate(pDC, -23, 0, false);
	DrawImgTransparent(pDC, legUp);

	Translate(pDC, legUp->Width(), legUp->Height(), false);
	Rotate(pDC, -alpha, false);
	Translate(pDC, -52, -45, false);
	DrawImgTransparent(pDC, legMiddle);

	
	Translate(pDC, legMiddle->Width(), legMiddle->Height(), false);
	Translate(pDC, -95, -15, false);
	DrawImgTransparent(pDC, foot);


	pDC->SetWorldTransform(&oldForm);
}

// 
// CTransporterView drawing

void CTransporterView::OnDraw(CDC* pDC)
{
	CTransporterDoc* pDoc = GetDocument();
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

	DrawScene(pMemDC, clientRect);

	pMemDC->SetWorldTransform(&oldForm);
	pMemDC->SetGraphicsMode(prevMode);
	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
	delete pMemDC;

	// TODO: add draw code for native data here
}


// CTransporterView printing

BOOL CTransporterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTransporterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTransporterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CTransporterView diagnostics

#ifdef _DEBUG
void CTransporterView::AssertValid() const
{
	CView::AssertValid();
}

void CTransporterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTransporterDoc* CTransporterView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTransporterDoc)));
	return (CTransporterDoc*)m_pDocument;
}
#endif //_DEBUG


// CTransporterView message handlers
void CTransporterView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT)
	{
		if (dir == 1)
			idx += 1;
		else
			idx -= 1;

		if (pdir == 1)
			pidx += 1;
		else
			pidx -= 1;

		if (idx == 4)
			dir = -1;
		if (idx == 0)
			dir = 1;

		if (pidx == 4)
			pdir = -1;
		if (pidx == 0)
			pdir = 1;

		z1 = a[idx];
		p1 = a[pidx];
	}
	else if (nChar == VK_UP)
		scaled += 0.1;
	else if (nChar == VK_DOWN)
		scaled -= 0.1;

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CTransporterView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}