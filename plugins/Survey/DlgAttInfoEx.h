#pragma once
#include "resource.h"
#include "CMyListCtr.h"

enum SHOWMODE{
	LEFT_TOP,
	LEFT_BOTTOM,
	LEFT_CENTER,
	RIGHT_TOP,
	RIGHT_BOTTOM,
	RIGHT_CENTER,
	LEFT,
	RIGHT
};
// CDlgAttInfoEx 对话框

class CDlgAttInfoEx : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAttInfoEx)

public:
	CDlgAttInfoEx(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAttInfoEx();

// 对话框数据
	enum { IDD = IDD_DIALOG_POINTSELECTEX };

	CFont m_font;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyListCtrl m_wndList;
	virtual BOOL OnInitDialog();
	void InsertRow(CString sName, CString sValue);
	void RemoveAllRows();
	void MovetoPoint();
	void SuitHeight(int len);
	void setPolygonRgn(SHOWMODE mode);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	RECT _rect;
};
