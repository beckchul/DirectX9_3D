
// AnimationTool.h : AnimationTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

// CAnimationToolApp:
// �� Ŭ������ ������ ���ؼ��� AnimationTool.cpp�� �����Ͻʽÿ�.
//

class CAnimationToolApp : public CWinAppEx
{
public:
	CAnimationToolApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CAnimationToolApp theApp;
