#pragma once
#include "resource.h"

// CAreaSurveyDlg �Ի���

class CAreaSurveyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAreaSurveyDlg)

public:
	CAreaSurveyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAreaSurveyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_AREA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	void SetArea(double , double);
	CObject* parent;
};
