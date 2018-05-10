#pragma once
#include <vector>
class CMyListCtrl : public CListCtrl
{
public:
	
	void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void SetRowHeigt(int nHeight);
	void setColor(COLORREF c)
	{
		color = c;
	}

	std::vector<UINT> indexVector;
	// Operations  
	void refreshIndex();

protected:
	int m_nRowHeight;
	COLORREF color;
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()
};