#pragma once

#include "Engine_Include.h"
#include "afxwin.h"

// CMyform 폼 뷰입니다.

class CMyform : public CFormView
{
	DECLARE_DYNCREATE(CMyform)

protected:
	CMyform();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyform();

public:
	map<int, Engine::ANI_INFO> m_mapAnimation;
	map<int, Engine::ANI_INFO> m_mapCromcruachAnimation;
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	Engine::ANI_INFO*	m_pAniInfo;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	float m_fSpeedfront;
	afx_msg void OnDeltaposSpinSpeedFront(NMHDR *pNMHDR, LRESULT *pResult);
	float m_fSpeedEnd;
	afx_msg void OnDeltaposSpinSpeedEnd(NMHDR *pNMHDR, LRESULT *pResult);
	float m_fWeightfront;
	afx_msg void OnDeltaposSpinWeightFront(NMHDR *pNMHDR, LRESULT *pResult);
	float m_fWeightEnd;
	afx_msg void OnDeltaposSpinWeightEnd(NMHDR *pNMHDR, LRESULT *pResult);
	double m_dMagicNumber;
	afx_msg void OnDeltaposSpinMagicnumber(NMHDR *pNMHDR, LRESULT *pResult);
	double m_dAniCount;
	afx_msg void OnDeltaposSpinAnicount(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPlay();
	int m_iAnimationIndex;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	CListBox m_AniInfolist;
	afx_msg void OnLbnSelchangeListAnimation();
	afx_msg void OnBnClickedButtonModify();
	CComboBox m_comboAniSelect;
	afx_msg void OnCbnSelchangeComboAniselect();
};


