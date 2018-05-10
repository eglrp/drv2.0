#pragma once

#include "vlc/vlc.h"

// CDLGVLCPlayer �Ի���

class CDLGVLCPlayer : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGVLCPlayer)

public:
	CDLGVLCPlayer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDLGVLCPlayer();

	void setRtspAddress(std::string path){rtspAddress = path;}

	void connectRTSP();
// �Ի�������
	enum { IDD = IDD_DIALOG_VLC };
private:
	libvlc_media_t *m_vlcMedia;
	libvlc_media_player_t *m_vlcMplay;
	libvlc_instance_t *m_vlcInst;

	CFont m_font;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CString m_title;
	CObject* parent;

private:
	std::string rtspAddress;
};
