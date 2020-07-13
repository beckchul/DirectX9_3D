
// MapToolView.h : CMapToolView 클래스의 인터페이스
//


#pragma once

#include "Engine_Function.h"
#include "Engine_Include.h"

namespace Engine
{
	class CGraphicDev;
	class CManagement;
	class CResourceMgr;
	class CTimeMgr;
	class CGameObject;
}

class CMapToolDoc;

class CMapToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMapToolView();
	DECLARE_DYNCREATE(CMapToolView)

// 특성입니다.
public:
	CMapToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMapToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	Engine::CGraphicDev*	m_pGraphicDev;
	Engine::CManagement*	m_pManagement;
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	Engine::CGameObject*	m_pCamera;
	Engine::CGameObject*	m_pBackGround;
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // MapToolView.cpp의 디버그 버전
inline CMapToolDoc* CMapToolView::GetDocument() const
   { return reinterpret_cast<CMapToolDoc*>(m_pDocument); }
#endif

