#pragma once
//#include "CWMPPlayer4.h"
#include "wmpplayer4.h"
#include "resource.h"

// CMediaPlayerDlg �Ի���

class CMediaPlayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMediaPlayerDlg)

public:
	CMediaPlayerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMediaPlayerDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PLAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	CWMPPlayer4 m_player;
	CObject* parent;
	afx_msg void OnPaint();
	CString m_title;
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnStnClickedStaticClose();
	//afx_msg void OnClose();

//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
