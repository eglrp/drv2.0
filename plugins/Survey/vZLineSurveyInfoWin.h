#pragma once

#include "resource.h"
//#include "vlLineSurveyTri.h"
// CvZLineSurveyInfoWin 对话框

class CvZLineSurveyInfoWin : public CDialogEx
{
	DECLARE_DYNAMIC(CvZLineSurveyInfoWin)

public:
	CvZLineSurveyInfoWin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CvZLineSurveyInfoWin();

	void SetDist(double , double , double);

// 对话框数据
	enum { IDD = IDD_DIALOG_LINESURVEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool m_bPath;
	CObject* parent;
	afx_msg void OnBnClickedCheckPathlinesurvey();
	afx_msg void OnClose();
};