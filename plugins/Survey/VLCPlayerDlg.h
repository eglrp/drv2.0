#pragma once
#include "resource.h"
#include "vlc/vlc.h" 

// CVLCPlayerDlg 对话框

class CVLCPlayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVLCPlayerDlg)

public:
	CVLCPlayerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVLCPlayerDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_VLCPLAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void setRtspAddress(std::string path)
	{	
		rtspAddress = path;	
	}

	void connectRTSP();
private:
	libvlc_media_t *m_vlcMedia;
	libvlc_media_player_t *m_vlcMplay;
	libvlc_instance_t *m_vlcInst;

	std::string rtspAddress;
	bool bpathSetted;
	CFont m_font;
public:
	virtual BOOL OnInitDialog();
	CString m_info;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
