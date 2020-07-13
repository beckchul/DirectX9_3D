
// MapToolView.cpp : CMapToolView 클래스의 구현
//

#include "stdafx.h"
#include "MapTool.h"

#include "MapToolDoc.h"
#include "MapToolView.h"

#include "MainFrm.h"
#include "Export_Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapToolView

IMPLEMENT_DYNCREATE(CMapToolView, CView)

BEGIN_MESSAGE_MAP(CMapToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMapToolView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMapToolView 생성/소멸

CMapToolView::CMapToolView()
: m_pGraphicDev(Engine::Get_GraphicDev())
, m_pManagement(Engine::Get_Management())
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMapToolView::~CMapToolView()
{
}

BOOL CMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMapToolView 그리기

void CMapToolView::OnDraw(CDC* /*pDC*/)
{
	CMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pTimeMgr->SetTime();

	Invalidate(FALSE);

	m_pDevice->Clear(0, NULL
		, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_XRGB(0, 0, 255), 1.f, 0);
	m_pDevice->BeginScene();


	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}


// CMapToolView 인쇄


void CMapToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMapToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMapToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CMapToolView 진단

#ifdef _DEBUG
void CMapToolView::AssertValid() const
{
	CView::AssertValid();
}

void CMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapToolDoc* CMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapToolDoc)));
	return (CMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapToolView 메시지 처리기


void CMapToolView::OnInitialUpdate()
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


	m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINCX, WINCY);

	m_pDevice = m_pGraphicDev->GetDevice();

	Engine::Get_FontMgr()->AddFont(m_pDevice, L"바탕", 28, 20, FW_NORMAL);
	Engine::Get_FontMgr()->AddFont(m_pDevice, L"궁서", 38, 28, FW_MEDIUM);
	Engine::Get_FontMgr()->AddFont(m_pDevice, L"굴림", 48, 38, FW_HEAVY);

	m_pManagement->InitManagement(m_pDevice);
	m_pResourceMgr->ReserveContainerSize(RESOURCE_END);

	m_pTimeMgr->InitTime();

	D3DLIGHT9		Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Direction = D3DXVECTOR3(1.f, -1.f, 1.f);
	Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Attenuation0 = 1.f;

	Engine::Get_LightMgr()->AddLight(m_pDevice, &Light, 0);

}


void CMapToolView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Engine::Safe_Single_Destory(m_pGraphicDev);
	Engine::Safe_Single_Destory(m_pTimeMgr);
	Engine::Get_FontMgr()->DestroyInstance();

	Engine::Safe_Single_Destory(m_pManagement);
	Engine::Get_LightMgr()->DestroyInstance();
	Engine::Safe_Single_Destory(m_pResourceMgr);
}


void CMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDown(nFlags, point);
}


void CMapToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
