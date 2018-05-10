#pragma once
 
#include "resource.h"
#include "afxwin.h"
#include "QRencode2Dlg.h"
class CPointSurveyInfoWin :
	public CDialogEx
{
	DECLARE_DYNAMIC(CPointSurveyInfoWin)

public:
	CPointSurveyInfoWin(CWnd* pParent = NULL);
	~CPointSurveyInfoWin(void);


	void SetText(std::string xtext,std::string ytext,std::string ztext,std::string ,std::string);
// 对话框数据
	enum { IDD = IDD_DIALOG_POINTSURVEY };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CQRencode2Dlg* pChild; 
	std::string slon,slat;
public: 
	CObject* parent; 
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CStatic m_Static;
	afx_msg void OnStnClickedStaticQc();
	afx_msg void OnBnClickedButtonQrencode1();
};

