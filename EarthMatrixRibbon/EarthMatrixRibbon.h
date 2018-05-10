// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// EarthMatrixRibbon.h : EarthMatrixRibbon Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CEarthMatrixRibbonApp:
// �йش����ʵ�֣������ EarthMatrixRibbon.cpp
//

class CEarthMatrixRibbonApp : public CWinAppEx
{
public:
	CEarthMatrixRibbonApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	BOOL GetExePath();
	void GetHWInfoFromDll();
	CString MD5(char* szSource);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	CString m_sInstallPath;
	CString m_sHWinfo;
};

extern CEarthMatrixRibbonApp theApp;
