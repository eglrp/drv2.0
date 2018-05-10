#if !defined(AFX_LINECOLORLISTCTRL_H__92EB1CD4_CD33_4E02_A098_367A8FEFF38E__INCLUDED_)  
#define AFX_LINECOLORLISTCTRL_H__92EB1CD4_CD33_4E02_A098_367A8FEFF38E__INCLUDED_  

#if _MSC_VER > 1000  
#pragma once  
#endif // _MSC_VER > 1000  
// LineColorListCtrl.h : header file  
//  

#include <afxtempl.h>  
#include <vector>

typedef struct    
{  
	COLORREF colText;  
	COLORREF colTextBk;  
}TEXT_BK;  

/////////////////////////////////////////////////////////////////////////////  
// CLineColorListCtrl window  

class CLineColorListCtrl : public CListCtrl  
{  
public:  
	void SetItemColor(DWORD iItem, COLORREF TextColor, COLORREF TextBkColor);   //设置某一行的前景色和背景色  
	void SetAllItemColor(DWORD iItem, COLORREF TextColor, COLORREF TextBkColor);//设置全部行的前景色和背景色  
	void ClearColor();                                                          //清除颜色映射表  

	// Construction  
public:  
	CLineColorListCtrl();  

	// Attributes  
public:  
	CMap<DWORD, DWORD&, TEXT_BK, TEXT_BK&> MapItemColor;  
	std::vector<DWORD> indexVector;
	// Operations  
	void refreshIndex();

public:  
	// Overrides  

	// Implementation  

public:  
	virtual ~CLineColorListCtrl();  
	// Generated message map functions  

protected:  
	void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()  

protected:
	COLORREF m_textColor;
	COLORREF m_textBkColor;
};  

#endif // !defined(AFX_LINECOLORLISTCTRL_H__92EB1CD4_CD33_4E02_A098_367A8FEFF38E__INCLUDED_)  