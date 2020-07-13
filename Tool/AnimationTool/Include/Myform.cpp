// Myform.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AnimationTool.h"
#include "Myform.h"
#include "AniArisha.h"
#include "AniCromcruach.h"
#include "Export_Function.h"
#include "SceneTool.h"
// CMyform

IMPLEMENT_DYNCREATE(CMyform, CFormView)

CMyform::CMyform()
: CFormView(IDD_MYFORM)
, m_fSpeedfront(1.f)
, m_fSpeedEnd(1.f)
, m_fWeightfront(0.7f)
, m_fWeightEnd(0.3f)
, m_dMagicNumber(0.25)
, m_dAniCount(0.4)
, m_iAnimationIndex(0)
{
	
}

CMyform::~CMyform()
{
}

void CMyform::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPEED_FRONT, m_fSpeedfront);
	DDX_Text(pDX, IDC_EDIT_SPEED_END, m_fSpeedEnd);
	DDX_Text(pDX, IDC_EDIT_WEIGHT_FRONT, m_fWeightfront);
	DDX_Text(pDX, IDC_EDIT_WEIGHT_END, m_fWeightEnd);
	DDX_Text(pDX, IDC_EDIT_MAGICNUMBER, m_dMagicNumber);
	DDX_Text(pDX, IDC_EDIT_ANICOUNT, m_dAniCount);
	DDX_Text(pDX, IDC_EDIT_ANIMATION_INDEX, m_iAnimationIndex);
	DDX_Control(pDX, IDC_LIST_ANIMATION, m_AniInfolist);
	DDX_Control(pDX, IDC_COMBO_ANISELECT, m_comboAniSelect);
}

BEGIN_MESSAGE_MAP(CMyform, CFormView)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED_FRONT, &CMyform::OnDeltaposSpinSpeedFront)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED_END, &CMyform::OnDeltaposSpinSpeedEnd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WEIGHT_FRONT, &CMyform::OnDeltaposSpinWeightFront)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WEIGHT_END, &CMyform::OnDeltaposSpinWeightEnd)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MAGICNUMBER, &CMyform::OnDeltaposSpinMagicnumber)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ANICOUNT, &CMyform::OnDeltaposSpinAnicount)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CMyform::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMyform::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMyform::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMyform::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMyform::OnBnClickedButtonLoad)
	ON_LBN_SELCHANGE(IDC_LIST_ANIMATION, &CMyform::OnLbnSelchangeListAnimation)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CMyform::OnBnClickedButtonModify)
	ON_CBN_SELCHANGE(IDC_COMBO_ANISELECT, &CMyform::OnCbnSelchangeComboAniselect)
END_MESSAGE_MAP()


// CMyform 진단입니다.

#ifdef _DEBUG
void CMyform::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyform::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyform 메시지 처리기입니다.


void CMyform::OnDeltaposSpinSpeedFront(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)
	{
		m_fSpeedfront += 0.01f;
	}
	else
	{
		if(m_fSpeedfront > 0.01f)
		m_fSpeedfront -= 0.01f;
	}
	*pResult = 0;
	UpdateData(FALSE);
}


void CMyform::OnDeltaposSpinSpeedEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)
	{
		m_fSpeedEnd += 0.01f;
	}
	else
	{
		if (m_fSpeedEnd > 0.01f)
			m_fSpeedEnd -= 0.01f;
	}
	*pResult = 0;
	UpdateData(FALSE);
}


void CMyform::OnDeltaposSpinWeightFront(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)
	{
		m_fWeightfront += 0.01f;
	}
	else
	{
		if (m_fWeightfront > 0.01f)
			m_fWeightfront -= 0.01f;
	}
	*pResult = 0;
	UpdateData(FALSE);
}


void CMyform::OnDeltaposSpinWeightEnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)
	{
		m_fWeightEnd += 0.01f;
	}
	else
	{
		if (m_fWeightEnd > 0.01f)
			m_fWeightEnd -= 0.01f;
	}
	*pResult = 0;
	UpdateData(FALSE);
}


void CMyform::OnDeltaposSpinMagicnumber(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)
	{
		m_dMagicNumber += 0.01f;
	}
	else
	{
		if (m_dMagicNumber > 0.01f)
			m_dMagicNumber -= 0.01f;
	}
	*pResult = 0;
	UpdateData(FALSE);
}


void CMyform::OnDeltaposSpinAnicount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	UpdateData(TRUE);
	if (pNMUpDown->iDelta < 0)
	{
		m_dAniCount += 0.01f;
	}
	else
	{
		if (m_dAniCount > 0.01f)
			m_dAniCount -= 0.01f;
	}
	*pResult = 0;
	UpdateData(FALSE);
}


void CMyform::OnBnClickedButtonPlay()
{
	UpdateData(TRUE);
	
	if(g_AniSelect == 0)
		m_pAniInfo = ((CAniArisha*)(Engine::Get_Management()->GetGameObjectlist(CSceneTool::LAYER_GAMELOGIC, L"AniArisha").front()))->GetAniInfo();
	else if(g_AniSelect == 1)
		m_pAniInfo = ((CAniCromcruach*)(Engine::Get_Management()->GetGameObjectlist(CSceneTool::LAYER_GAMELOGIC, L"AniCromcruach").front()))->GetAniInfo();

	m_pAniInfo->iAniIndex		= m_iAnimationIndex;
	m_pAniInfo->dMagicNumber	= m_dMagicNumber;
	m_pAniInfo->dEndAniCount	= m_dAniCount;
	m_pAniInfo->fAniSpeedFront	= m_fSpeedfront;
	m_pAniInfo->fAniSpeedEnd	= m_fSpeedEnd;
	m_pAniInfo->fAniWeightFront = m_fWeightfront;
	m_pAniInfo->fAniWeightEnd	= m_fWeightEnd;

	if (g_AniSelect == 0)
		((CAniArisha*)(Engine::Get_Management()->GetGameObjectlist(CSceneTool::LAYER_GAMELOGIC, L"AniArisha").front()))->AnimationStart();
	else if(g_AniSelect == 1)
		((CAniCromcruach*)(Engine::Get_Management()->GetGameObjectlist(CSceneTool::LAYER_GAMELOGIC, L"AniCromcruach").front()))->AnimationStart();

	UpdateData(FALSE);
}


void CMyform::OnBnClickedButtonAdd()
{
	UpdateData(TRUE);

	if (g_AniSelect == 0)
	{
		map<int, Engine::ANI_INFO>::iterator iter = m_mapAnimation.find(m_iAnimationIndex);
		if (iter == m_mapAnimation.end())
		{
			Engine::ANI_INFO tTemp;

			tTemp.iAniIndex = m_iAnimationIndex;
			tTemp.dMagicNumber = m_dMagicNumber;
			tTemp.dEndAniCount = m_dAniCount;
			tTemp.fAniSpeedFront = m_fSpeedfront;
			tTemp.fAniSpeedEnd = m_fSpeedEnd;
			tTemp.fAniWeightFront = m_fWeightfront;
			tTemp.fAniWeightEnd = m_fWeightEnd;

			m_mapAnimation.insert(make_pair(m_iAnimationIndex, tTemp));

			TCHAR szBuf[64] = L"";
			_itow_s(m_iAnimationIndex, szBuf, 10);
			m_AniInfolist.AddString(szBuf);
			UpdateData(FALSE);
			return;
		}
		else
		{
			MessageBox(L"이미 존재하는 애니메이션 인덱스 입니다.", NULL, MB_OK);
			UpdateData(FALSE);
			return;
		}
	}
	else if (g_AniSelect == 1)
	{
		map<int, Engine::ANI_INFO>::iterator iter = m_mapCromcruachAnimation.find(m_iAnimationIndex);
		if (iter == m_mapCromcruachAnimation.end())
		{
			Engine::ANI_INFO tTemp;

			tTemp.iAniIndex = m_iAnimationIndex;
			tTemp.dMagicNumber = m_dMagicNumber;
			tTemp.dEndAniCount = m_dAniCount;
			tTemp.fAniSpeedFront = m_fSpeedfront;
			tTemp.fAniSpeedEnd = m_fSpeedEnd;
			tTemp.fAniWeightFront = m_fWeightfront;
			tTemp.fAniWeightEnd = m_fWeightEnd;

			m_mapCromcruachAnimation.insert(make_pair(m_iAnimationIndex, tTemp));

			TCHAR szBuf[64] = L"";
			_itow_s(m_iAnimationIndex, szBuf, 10);
			m_AniInfolist.AddString(szBuf);
			UpdateData(FALSE);
			return;
		}
		else
		{
			MessageBox(L"이미 존재하는 애니메이션 인덱스 입니다.", NULL, MB_OK);
			UpdateData(FALSE);
			return;
		}
	}
}


void CMyform::OnBnClickedButtonDelete()
{
	UpdateData(TRUE);

	CString strSelectName;

	int iSelectIndex = m_AniInfolist.GetCurSel();

	if (iSelectIndex < 0)
		return;

	m_AniInfolist.GetText(iSelectIndex, strSelectName);

	int iDeleteIndex = _ttoi(strSelectName);

	if(g_AniSelect == 0)
	{ 
		map<int, Engine::ANI_INFO>::iterator iter = m_mapAnimation.find(iDeleteIndex);
		if (iter == m_mapAnimation.end())
		{
			MessageBox(L"존재하지 않는 애니메이션 인덱스 입니다.(오류)", NULL, MB_OK);
			UpdateData(FALSE);
			return;
		}
		m_mapAnimation.erase(iDeleteIndex);
	}
	else if (g_AniSelect == 1)
	{
		map<int, Engine::ANI_INFO>::iterator iter = m_mapCromcruachAnimation.find(iDeleteIndex);
		if (iter == m_mapCromcruachAnimation.end())
		{
			MessageBox(L"존재하지 않는 애니메이션 인덱스 입니다.(오류)", NULL, MB_OK);
			UpdateData(FALSE);
			return;
		}
		m_mapCromcruachAnimation.erase(iDeleteIndex);
	}
	m_AniInfolist.DeleteString(iSelectIndex);
	UpdateData(FALSE);
}

void CMyform::OnLbnSelchangeListAnimation()
{
	UpdateData(TRUE);

	CString strSelectName;

	int iSelectIndex = m_AniInfolist.GetCurSel();
	if (iSelectIndex == -1)
		return;

	m_AniInfolist.GetText(iSelectIndex, strSelectName);
	
	iSelectIndex = _ttoi(strSelectName);

	if (g_AniSelect == 0)
	{
		map<int, Engine::ANI_INFO>::iterator iter = m_mapAnimation.find(iSelectIndex);
		if (iter == m_mapAnimation.end())
		{
			MessageBox(L"없음", NULL, MB_OK);
			return;
		}

		m_iAnimationIndex = iter->second.iAniIndex;
		m_fSpeedfront = iter->second.fAniSpeedFront;
		m_fSpeedEnd = iter->second.fAniSpeedEnd;
		m_fWeightfront = iter->second.fAniWeightFront;
		m_fWeightEnd = iter->second.fAniWeightEnd;
		m_dMagicNumber = iter->second.dMagicNumber;
		m_dAniCount = iter->second.dEndAniCount;

	}
	else if(g_AniSelect == 1)
	{
		map<int, Engine::ANI_INFO>::iterator iter = m_mapCromcruachAnimation.find(iSelectIndex);
		if (iter == m_mapCromcruachAnimation.end())
		{
			MessageBox(L"없음", NULL, MB_OK);
			return;
		}

		m_iAnimationIndex = iter->second.iAniIndex;
		m_fSpeedfront = iter->second.fAniSpeedFront;
		m_fSpeedEnd = iter->second.fAniSpeedEnd;
		m_fWeightfront = iter->second.fAniWeightFront;
		m_fWeightEnd = iter->second.fAniWeightEnd;
		m_dMagicNumber = iter->second.dMagicNumber;
		m_dAniCount = iter->second.dEndAniCount;

	}

	UpdateData(FALSE);
}


void CMyform::OnBnClickedButtonModify()
{
	UpdateData(TRUE);
	OnBnClickedButtonDelete();
	OnBnClickedButtonAdd();
	UpdateData(FALSE);
}

void CMyform::OnBnClickedButtonSave()
{
	if (g_AniSelect == 0)
	{
		HANDLE hFile = CreateFile(L"../../../Client/bin/Data/ArishaAnimation.bat",
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		DWORD dwByte;

		for (map<int, Engine::ANI_INFO>::iterator iter = m_mapAnimation.begin();
			iter != m_mapAnimation.end(); ++iter)
		{
			WriteFile(hFile, &(iter->second), sizeof(Engine::ANI_INFO), &dwByte, NULL);
		}

		CloseHandle(hFile);
	}
	else if (g_AniSelect == 1)
	{
		HANDLE hFile = CreateFile(L"../../../Client/bin/Data/CromcruachAnimation.bat",
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		DWORD dwByte;

		for (map<int, Engine::ANI_INFO>::iterator iter = m_mapCromcruachAnimation.begin();
			iter != m_mapCromcruachAnimation.end(); ++iter)
		{
			WriteFile(hFile, &(iter->second), sizeof(Engine::ANI_INFO), &dwByte, NULL);
		}

		CloseHandle(hFile);
	}
}


void CMyform::OnBnClickedButtonLoad()
{
	UpdateData(TRUE);

	if (g_AniSelect == 0)
	{
		HANDLE hFile = CreateFile(L"../../../Client/bin/Data/ArishaAnimation.bat",
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		DWORD dwByte;
		m_mapAnimation.clear();
		m_AniInfolist.ResetContent();

		while (true)
		{
			Engine::ANI_INFO AniData;

			ReadFile(hFile, &AniData, sizeof(Engine::ANI_INFO), &dwByte, NULL);

			if (dwByte == 0)
			{
				break;
			}

			m_mapAnimation.insert(make_pair(AniData.iAniIndex, AniData));

			TCHAR szBuf[64] = L"";
			_itow_s(AniData.iAniIndex, szBuf, 10);
			m_AniInfolist.AddString(szBuf);
		}


		CloseHandle(hFile);
	}
	else if (g_AniSelect == 1)
	{
		HANDLE hFile = CreateFile(L"../../../Client/bin/Data/CromcruachAnimation.bat",
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		DWORD dwByte;
		m_mapCromcruachAnimation.clear();
		m_AniInfolist.ResetContent();

		while (true)
		{
			Engine::ANI_INFO AniData;

			ReadFile(hFile, &AniData, sizeof(Engine::ANI_INFO), &dwByte, NULL);

			if (dwByte == 0)
			{
				break;
			}

			m_mapCromcruachAnimation.insert(make_pair(AniData.iAniIndex, AniData));

			TCHAR szBuf[64] = L"";
			_itow_s(AniData.iAniIndex, szBuf, 10);
			m_AniInfolist.AddString(szBuf);
		}


		CloseHandle(hFile);
	}

	UpdateData(FALSE);
}


void CMyform::OnCbnSelchangeComboAniselect()
{
	UpdateData(TRUE);

	int iSel = m_comboAniSelect.GetCurSel();

	g_AniSelect = iSel;
	m_AniInfolist.ResetContent();

	if (g_AniSelect == 0)
	{
		for (map<int, Engine::ANI_INFO>::iterator iter = m_mapAnimation.begin();
			iter != m_mapAnimation.end(); ++iter)
		{
			TCHAR szBuf[64] = L"";
			_itow_s(iter->second.iAniIndex, szBuf, 10);
			m_AniInfolist.AddString(szBuf);
		}
	}
	else if (g_AniSelect == 1)
	{
		for (map<int, Engine::ANI_INFO>::iterator iter = m_mapCromcruachAnimation.begin();
			iter != m_mapCromcruachAnimation.end(); ++iter)
		{
			TCHAR szBuf[64] = L"";
			_itow_s(iter->second.iAniIndex, szBuf, 10);
			m_AniInfolist.AddString(szBuf);
		}
	}
	

	UpdateData(FALSE);
}
