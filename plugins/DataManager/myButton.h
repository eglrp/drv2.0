#pragma once
#include "afxdialogex.h"
class MyButton : public CButton
{
private:
	bool is_pressed;  //鼠标是否按下  
public:
	virtual void PreSubclassWindow();  //修改自绘属性  
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);  //绘制 圆形按钮  
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);  //鼠标按下  
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);  //鼠标松开  
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);  //不擦除背景  
};