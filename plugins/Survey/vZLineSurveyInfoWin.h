#pragma once

#include "resource.h"
//#include "vlLineSurveyTri.h"
// CvZLineSurveyInfoWin �Ի���

class CvZLineSurveyInfoWin : public CDialogEx
{
	DECLARE_DYNAMIC(CvZLineSurveyInfoWin)

public:
	CvZLineSurveyInfoWin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CvZLineSurveyInfoWin();

	void SetDist(double , double , double);

// �Ի�������
	enum { IDD = IDD_DIALOG_LINESURVEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool m_bPath;
	CObject* parent;
	afx_msg void OnBnClickedCheckPathlinesurvey();
	afx_msg void OnClose();
};