#pragma once
#include "afxdialogex.h"
class MyButton : public CButton
{
private:
	bool is_pressed;  //����Ƿ���  
public:
	virtual void PreSubclassWindow();  //�޸��Ի�����  
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);  //���� Բ�ΰ�ť  
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);  //��갴��  
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  //����ɿ�  
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);  //����������  
};