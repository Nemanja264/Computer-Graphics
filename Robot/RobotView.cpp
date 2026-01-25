
// RobotView.cpp : implementation of the CRobotView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Robot.h"
#endif

#include "RobotDoc.h"
#include "RobotView.h"
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRobotView

IMPLEMENT_DYNCREATE(CRobotView, CView)

BEGIN_MESSAGE_MAP(CRobotView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CRobotView construction/destruction

CRobotView::CRobotView() noexcept
{
	// TODO: add construction code here
	pozadina->Load(CString("./pozadina.jpg"));
	telo->Load(CString("./telo.png"));
	glava->Load(CString("./glava.png"));
	nadkolenica->Load(CString("./nadkolenica.png"));
	nadlaktica->Load(CString("./nadlaktica.png"));
	podkolenica->Load(CString("./podkolenica.png"));
	podlaktica->Load(CString("./podlaktica.png"));
	saka->Load(CString("./saka.png"));
	stopalo->Load(CString("./stopalo.png"));
}

CRobotView::~CRobotView()
{
	delete glava;
	delete telo;
	delete saka;
	delete stopalo;
	delete pozadina;
	delete nadkolenica;
	delete nadlaktica;
	delete podlaktica;
	delete podkolenica;
}

BOOL CRobotView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CRobotView::Scale(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CRobotView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CRobotView::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void CRobotView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	BYTE* bytes = pImage->GetDIBBits();
	COLORREF color = RGB(bytes[2], bytes[1], bytes[0]);

	int w = pImage->Width();
	int h = pImage->Height();
	CRect rect(0, 0, w, h);

	pImage->DrawTransparent(pDC, rect, rect, color);
}

void CRobotView::DrawBackground(CDC* pDC) {
	int w = pozadina->Width();
	int h = pozadina->Height();
	CRect imgRect(0, 0, w, h);

	CRect clientRect;
	GetClientRect(&clientRect);

	CPoint topleft(clientRect.Width() / 2 - w / 2, clientRect.Height() - h);
	CPoint rightBottom(clientRect.Width() / 2 + w / 2, clientRect.Height());
	CRect dstRect(topleft, rightBottom);

	pozadina->Draw(pDC, imgRect,imgRect);
}
void CRobotView::DrawHalf(CDC* pDC)
{

	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, -telo->Width(), 0, false);
	DrawImgTransparent(pDC, telo);

	Translate(pDC, 25, 65, false);
	Rotate(pDC, rotNadLaktica, false);
	Translate(pDC, -35, -35, false);
	DrawImgTransparent(pDC, nadlaktica);


	Translate(pDC, 22, 167, false);
	Rotate(pDC, rotPodLaktica, false);
	Translate(pDC, -30, -33, false);
	DrawImgTransparent(pDC, podlaktica);


	Translate(pDC, 30, 140, false);
	Rotate(pDC, rotSaka, false);
	Translate(pDC, -25, -3, false);
	DrawImgTransparent(pDC, saka);

	pDC->SetWorldTransform(&oldForm);
	Translate(pDC, -telo->Width(), 0, false);


	Translate(pDC, 61, 262, false);
	Translate(pDC, -29, -20, false);
	DrawImgTransparent(pDC, nadkolenica);


	Translate(pDC, 30, 184, false);
	Translate(pDC, -25, -37, false);
	DrawImgTransparent(pDC, podkolenica);


	Translate(pDC, 25, 248, false);
	Translate(pDC, -20, -16, false);
	DrawImgTransparent(pDC, stopalo);

	pDC->SetWorldTransform(&oldForm);
}

void CRobotView::DrawRobot(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 0, -telo->Height(), false);

	DrawHalf(pDC);
	Scale(pDC, -1, 1, false);
	DrawHalf(pDC);

	pDC->SetWorldTransform(&oldForm);


	Translate(pDC, 0, -telo->Height(), false);
	Translate(pDC, -glava->Width()/2, -glava->Height()/2-40, false);

	DrawImgTransparent(pDC, glava);


	pDC->SetWorldTransform(&oldForm);
}
// CRobotView drawing

void CRobotView::OnDraw(CDC* pDC)
{
	CRobotDoc* pDoc = GetDocument();
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
	XFORM oldform;
	pMemDC->GetWorldTransform(&oldform);

	DrawBackground(pMemDC);

	float x = pozadina->Width() / 2;
	float y = pozadina->Height()/2;

	Translate(pMemDC, x, y, false);
	Rotate(pMemDC, ceo, false);
	Scale(pMemDC, skaliranje, skaliranje, false);
	DrawRobot(pMemDC);

	pMemDC->SetWorldTransform(&oldform);

	pDC->BitBlt(0, 0, clientRect.Width(), clientRect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->DeleteDC();
	delete pMemDC;
	// TODO: add draw code for native data here
}


// CRobotView printing

BOOL CRobotView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRobotView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRobotView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRobotView diagnostics

#ifdef _DEBUG
void CRobotView::AssertValid() const
{
	CView::AssertValid();
}

void CRobotView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRobotDoc* CRobotView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRobotDoc)));
	return (CRobotDoc*)m_pDocument;
}
#endif //_DEBUG

void CRobotView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ((nChar == 'A' || nChar == 'a') && rotSaka >= 0)
	{
		rotSaka -= 5;
	}
	else if ((nChar == 'S' || nChar == 's') && rotSaka <= 20)
	{
		rotSaka += 5;
	}
	else if ((nChar == 'D' || nChar == 'd') && rotPodLaktica >=0)
	{
		rotPodLaktica -= 5;
	}
	else if ((nChar == 'F' || nChar == 'f') && rotPodLaktica <= 70)
	{
		rotPodLaktica += 5;
	}
	else if ((nChar == 'G' || nChar == 'g') && rotNadLaktica >= 0)
	{
		rotNadLaktica -= 5;
	}
	else if ((nChar == 'H' || nChar == 'h') && rotNadLaktica <= 80)
	{
		rotNadLaktica += 5;
	}

	if (nChar == '1')
		skaliranje += 0.1;
	else if(nChar == '2')
		skaliranje -= 0.1;


	if (nChar == '3')
		ceo += 5;
	else if (nChar == '4')
		ceo -= 5;


	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CRobotView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
// CRobotView message handlers
