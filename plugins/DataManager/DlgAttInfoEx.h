#pragma once
#include "Resource.h"
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
	CMyListCtrl	m_wndList;
// 对话框数据
	enum { IDD = IDD_DIALOG_ENTINFOEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CFont m_font;
public:
	virtual BOOL OnInitDialog();

	void InsertRow(CString sName, CString sValue);
	void RemoveAllRows();
	void MovetoPoint();
	void SuitHeight(int len);
	void setPolygonRgn(SHOWMODE mode);

	afx_msg void OnPaint();
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	RECT _rect;
};
