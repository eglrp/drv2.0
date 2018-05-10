#pragma once
#include "afxcmn.h"
class CTreeCtrlX :
	public CTreeCtrl
{
public:
	CTreeCtrlX(void);
	~CTreeCtrlX(void);
public:
	struct Color_Font
	{
		COLORREF color;
		LOGFONT logfont;
	};
	CMap< void*, void*, Color_Font, Color_Font& > m_mapColorFont;
	void SetItemFont(HTREEITEM hItem, LOGFONT& logfont);
	void SetItemBold(HTREEITEM hItem, BOOL bBold);
	void SetItemColor(HTREEITEM hItem, COLORREF color);
	BOOL GetItemFont(HTREEITEM hItem, LOGFONT * plogfont);
	BOOL GetItemBold(HTREEITEM hItem);
	COLORREF GetItemColor(HTREEITEM hItem);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};

