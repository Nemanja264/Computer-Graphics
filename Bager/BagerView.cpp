
// BagerView.cpp : implementation of the CBagerView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Bager.h"
#endif

#include "BagerDoc.h"
#include "BagerView.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBagerView

IMPLEMENT_DYNCREATE(CBagerView, CView)

BEGIN_MESSAGE_MAP(CBagerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CBagerView construction/destruction

CBagerView::CBagerView() noexcept
{
	// TODO: add construction code here
	pozadina->Load(CString("./pozadina.png"));
	bager->Load(CString("./bager.png"));
	arm1->Load(CString("./arm1.png"));
	arm2->Load(CString("./arm2.png"));

	hViljuska = GetEnhMetaFile(L"./viljuska.emf");


	if (hViljuska)
	{
		ENHMETAHEADER hdr = { 0 };
		hdr.iType = EMR_HEADER;
		hdr.nSize = sizeof(ENHMETAHEADER);

		if (GetEnhMetaFileHeader(hViljuska, sizeof(hdr), &hdr))
		{
			HDC hdc = ::GetDC(nullptr);
			int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
			int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
			::ReleaseDC(nullptr, hdc);

			auto mm100_to_px_x = [&](int v) -> int { return MulDiv(v, dpiX, 2540); };
			auto mm100_to_px_y = [&](int v) -> int { return MulDiv(v, dpiY, 2540); };

			viljuskaFramePx = CRect(
				mm100_to_px_x(hdr.rclFrame.left),
				mm100_to_px_y(hdr.rclFrame.top),
				mm100_to_px_x(hdr.rclFrame.right),
				mm100_to_px_y(hdr.rclFrame.bottom)
			);
		}
	}

}

CBagerView::~CBagerView()
{
	delete pozadina;
	delete bager;
	delete arm1;
	delete arm2;

	if (hViljuska)
		DeleteEnhMetaFile(hViljuska);
}

BOOL CBagerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CBagerView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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
void CBagerView::Rotate(CDC* pDC, float angle, bool rightMultiply)
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
void CBagerView::Scale(CDC* pDC, float dX, float dY, bool rightMultiply)
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
void CBagerView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);

	float w = pImage->Width();
	float h = pImage->Height();
	CRect imgRect(0, 0, w, h);


	pImage->DrawTransparent(pDC, imgRect, imgRect, color);
}
void CBagerView::DrawBackground(CDC* pDC)
{
	float w = pozadina->Width();
	float h = pozadina->Height();
	CRect imgRect(0, 0, w, h);

	CRect clientRect;
	GetClientRect(&clientRect);

	CPoint topLeft(clientRect.Width() / 2 - w / 2, clientRect.Height() - h);
	CPoint bottomRight(clientRect.Width() / 2 + w / 2, clientRect.Height());
	CRect dstRect(topLeft, bottomRight);

	pozadina->Draw(pDC, imgRect, dstRect);
}

void CBagerView::DrawBody(CDC* pDC)
{
	Translate(pDC, -bager->Width(), -bager->Height(), false);
	DrawImgTransparent(pDC, bager);
}

void CBagerView::DrawArm1(CDC* pDC)
{
	Translate(pDC, 62, 170, false);
	Rotate(pDC, rotArm1, false);
	Translate(pDC, -58, -61, false);
	DrawImgTransparent(pDC, arm1);
}


void CBagerView::DrawArm2(CDC* pDC)
{
	Translate(pDC, 309, 61, false);
	Rotate(pDC, rotArm2, false);
	Translate(pDC, -36, -40, false);
	DrawImgTransparent(pDC, arm2);	
}

void CBagerView::DrawFork(CDC* pDC)
{
	Translate(pDC, 272, 40, false);
	Rotate(pDC, rotViljuska, false);
	Scale(pDC, 2.5, 2.5, false);
	Translate(pDC, -14, -20, false);
	CRect src = viljuskaFramePx;
	CRect dst(0, 0, src.Width(), src.Height());

	PlayEnhMetaFile(pDC->m_hDC, hViljuska, &dst);

}

void CBagerView::DrawExcavator(CDC* pDC)
{
	Translate(pDC, bagerLeftRight, 0, false);

	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);

	pDC->SetWorldTransform(&oldForm);
}


// CBagerView drawing

void CBagerView::OnDraw(CDC* pDC)
{
	CBagerDoc* pDoc = GetDocument();
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

	DrawBackground(pMemDC);

	// ovo sam stavio za slucaj da bager treba da bude u donjem desnom uglu pozadine a ne client Windowa
	float w = pozadina->Width();
	float h = pozadina->Height();
	float x = clientRect.Width() / 2 + w / 2;
	float y = clientRect.Height();

	Translate(pMemDC, clientRect.Width(), y, false);
	DrawExcavator(pMemDC);

	pMemDC->SetWorldTransform(&oldForm);
	pMemDC->SetGraphicsMode(prevMode);
	
	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
	delete pMemDC;

	// TODO: add draw code for native data here
}


// CBagerView printing

BOOL CBagerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBagerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBagerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBagerView diagnostics

#ifdef _DEBUG
void CBagerView::AssertValid() const
{
	CView::AssertValid();
}

void CBagerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBagerDoc* CBagerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBagerDoc)));
	return (CBagerDoc*)m_pDocument;
}
#endif //_DEBUG

void CBagerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == '1')
		rotArm1 += 10;
	else if (nChar == '2')
		rotArm1 -= 10;
	else if (nChar == '3')
		rotArm2 += 10;
	else if (nChar == '4')
		rotArm2 -= 10;
	else if (nChar == '5')
		rotViljuska += 10;
	else if (nChar == '6')
		rotViljuska -= 10;
	else if (nChar == VK_LEFT)
		bagerLeftRight -= 10;
	else if (nChar == VK_RIGHT)
		bagerLeftRight += 10;

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CBagerView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
// CBagerView message handlers
