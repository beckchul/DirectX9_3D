
// AnimationToolView.h : CAnimationToolView Ŭ������ �������̽�
//

#pragma once


class CAnimationToolDoc;

class CAnimationToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CAnimationToolView();
	DECLARE_DYNCREATE(CAnimationToolView)

// Ư���Դϴ�.
public:
	CAnimationToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CAnimationToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
};

#ifndef _DEBUG  // AnimationToolView.cpp�� ����� ����
inline CAnimationToolDoc* CAnimationToolView::GetDocument() const
   { return reinterpret_cast<CAnimationToolDoc*>(m_pDocument); }
#endif

