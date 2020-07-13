
// AnimationToolView.cpp : CAnimationToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "AnimationTool.h"
#endif

#include "AnimationToolDoc.h"
#include "AnimationToolView.h"

#include "MainFrm.h"
#include "AnimationMainApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimationToolView

IMPLEMENT_DYNCREATE(CAnimationToolView, CView)

BEGIN_MESSAGE_MAP(CAnimationToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CAnimationToolView ����/�Ҹ�

CAnimationToolView::CAnimationToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CAnimationToolView::~CAnimationToolView()
{
}

BOOL CAnimationToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CAnimationToolView �׸���

void CAnimationToolView::OnDraw(CDC* /*pDC*/)
{
	CAnimationToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

}


// CAnimationToolView �μ�

BOOL CAnimationToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CAnimationToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CAnimationToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CAnimationToolView ����

#ifdef _DEBUG
void CAnimationToolView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimationToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimationToolDoc* CAnimationToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimationToolDoc)));
	return (CAnimationToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimationToolView �޽��� ó����


void CAnimationToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	RECT rcRect;

	pMainFrame->GetWindowRect(&rcRect);

	SetRect(&rcRect, 0, 0, rcRect.right - rcRect.left, rcRect.bottom - rcRect.top);

	RECT rcMainView;
	GetClientRect(&rcMainView);

	pMainFrame->SetWindowPos(NULL, 0, 0
		, WINCX + rcRect.right - rcMainView.right
		, WINCY + rcRect.bottom - rcMainView.bottom
		, SWP_NOZORDER);

	g_hWnd = m_hWnd;
}


void CAnimationToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
