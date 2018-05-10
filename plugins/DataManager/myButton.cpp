#include "stdafx.h"
#include "myButton.h"

void MyButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);
	is_pressed = false;
	CButton::PreSubclassWindow();
}

void MyButton::DrawItem(LPDRAWITEMSTRUCT lp)
{
	CRect rc = lp->rcItem;
	CDC dc;
	dc.Attach(lp->hDC);
	dc.SetBkMode(TRANSPARENT);
	CBrush br;
	br.CreateStockObject(NULL_BRUSH);
	dc.SelectObject(&br);
	if (is_pressed)
	{
		CPen pen(PS_SOLID, 2, RGB(225, 225,225));
		dc.SelectObject(&pen);
		dc.Ellipse(0, 0, rc.Width(), rc.Height());
		dc.SetTextColor(RGB(255, 225, 225));
	}
	else
	{
		CPen pen(PS_SOLID, 2, RGB(255, 255, 255));
		dc.SelectObject(&pen);
		dc.Ellipse(0, 0, rc.Width(), rc.Height());
		dc.SetTextColor(RGB(255, 255, 255));
	}
	CString str;
	GetWindowText(str);
	dc.DrawText(str, CRect(0, 0, rc.right, rc.bottom), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	dc.DeleteDC();
}

BEGIN_MESSAGE_MAP(MyButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void MyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	is_pressed = true;
	CButton::OnLButtonDown(nFlags, point);
}

void MyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	is_pressed = false;
	CButton::OnLButtonUp(nFlags, point);
}

BOOL MyButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;  //²»²Á³ý±³¾°  
}