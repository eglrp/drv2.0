// Author:      SFr frydaysoft@gmx.de
// Download by http://www.NewXing.com
#include "stdafx.h"
#include "ColorListCtrl.h"
#include <algorithm>
#ifdef _DEBUG  
#define new DEBUG_NEW  
#undef THIS_FILE  
static char THIS_FILE[] = __FILE__;  
#endif  

/////////////////////////////////////////////////////////////////////////////  
// CLineColorListCtrl  

CLineColorListCtrl::CLineColorListCtrl()  
{  
}  

CLineColorListCtrl::~CLineColorListCtrl()  
{  
}  


BEGIN_MESSAGE_MAP(CLineColorListCtrl, CListCtrl)  
	//{{AFX_MSG_MAP(CLineColorListCtrl)  
	// NOTE - the ClassWizard will add and remove mapping macros here.  
	//}}AFX_MSG_MAP 
	ON_WM_DRAWITEM()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)  
END_MESSAGE_MAP()  

/////////////////////////////////////////////////////////////////////////////  
// CLineColorListCtrl message handlers  

void CLineColorListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)    
{    
	// TODO: Add your control notification handler code here     
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = CDRF_DODEFAULT;

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF clrNewTextColor, clrNewBkColor;
		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		//设置背景色
		//if( nItem%2 ==0 )
		//{
		//	clrNewBkColor = RGB( 240, 240, 240 ); //偶数行背景色为灰色
		//}
		//else
		//{
		//	clrNewBkColor = RGB( 255, 255, 255 ); //奇数行背景色为白色
		//}
		clrNewBkColor = RGB( 240, 0, 0 ); 

		//如果在indexVector,则显示字体为红色,否则为黑色
		if (find(indexVector.begin(),indexVector.end(),nItem) != indexVector.end())
		{
			clrNewTextColor = RGB( 255, 0, 0 );
		}
		else 
		{
			clrNewTextColor = RGB( 255, 255, 255 );
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}
}    

void CLineColorListCtrl::ClearColor()  
{  
	MapItemColor.RemoveAll();  
}  

void CLineColorListCtrl::SetItemColor(DWORD iItem, COLORREF TextColor, COLORREF TextBkColor)  
{    
	TEXT_BK tb;  
	tb.colText = TextColor;  
	tb.colTextBk = TextBkColor;  


	MapItemColor.SetAt(iItem, tb);//设置某行的颜色。     
	this->RedrawItems(iItem, iItem);//重新染色     

	//this->SetCheck(iItem,1);     
	this->SetFocus();    //设置焦点     
	UpdateWindow();    
}    


void CLineColorListCtrl::SetAllItemColor(DWORD iItem, COLORREF TextColor, COLORREF TextBkColor)    
{    
	//INT_PTR ncount = this->GetItemCount();  
	TEXT_BK tb;  
	tb.colText = TextColor;  
	tb.colTextBk = TextBkColor;  

	if(iItem > 0)    
	{    
		for(DWORD numItem = 0; numItem < iItem ;numItem ++)    
		{    
			//iItem = i;     
			MapItemColor.SetAt(numItem, tb);    
			this->RedrawItems(numItem, numItem);    
		}    
	}    
	return;    
}  

void CLineColorListCtrl::refreshIndex()
{
	USES_CONVERSION;
	CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
	int nCol = pHeaderCtrl->GetItemCount();//列数

	indexVector.clear();
	int nRow = GetItemCount();
	for (int i = 0;i<nRow;++i)
	{
		for (int j = 0;j<nCol;++j)
		{
			std::string s(W2A(GetItemText(i,j)));
			if (s.find("异常") != -1)
			{
				indexVector.push_back(i);
				break;
			}
		}
	}
}

void CLineColorListCtrl::DrawItem(LPDRAWITEMSTRUCT lpMeasureItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpMeasureItemStruct->hDC);    
	LVITEM lvi = {0}; 
	lvi.mask = LVIF_STATE;//|LVIF_IMAGE; 
	lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED ; 
	lvi.iItem = lpMeasureItemStruct->itemID; 
	BOOL bGet = GetItem(&lvi); 
	//高亮显示
	BOOL bHighlight =((lvi.state & LVIS_DROPHILITED)||((lvi.state & LVIS_SELECTED) && 
		((GetFocus() == this)|| (GetStyle() & LVS_SHOWSELALWAYS)))); 
	// 画文本背景 
	CRect rcBack = lpMeasureItemStruct->rcItem; 
	pDC->SetBkMode(TRANSPARENT); 
	if (bHighlight)
	{
		pDC->SetTextColor(RGB(250,250,250));      
		pDC->FillRect(rcBack, &CBrush(RGB(240,0,0)));
	}
	else
	{
		pDC->SetTextColor(RGB(255,255,255));      
		pDC->FillRect(rcBack, &CBrush(RGB(240,0,0))); 
	}
	if (lpMeasureItemStruct->itemAction & ODA_DRAWENTIRE) 
	{ 
		//写文本 
		CString szText; 
		int nCollumn = GetHeaderCtrl()->GetItemCount();//列数
		for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++) 
		{ //循环得到文本 
			CRect rcItem; 
			if ( !GetSubItemRect(lpMeasureItemStruct->itemID, i, LVIR_LABEL, rcItem )) 
				continue; 
			szText = GetItemText( lpMeasureItemStruct->itemID, i ); 
			rcItem.left += 5; rcItem.right -= 1; 
			pDC->DrawText(szText, lstrlen(szText), &rcItem,  DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
		} 
	} 

}