#pragma once
#include "resource.h"
#include "afxwin.h"

// CShowPictureDlg 对话框

class CShowPictureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowPictureDlg)

public:
	CShowPictureDlg(bool isTranparent = true,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowPictureDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PICTUREINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:	
	void setPath(std::string path);                    //设置图片路径
	void setColorType(int c);                          //设置窗口背景颜色
	void changePictureSize(int w,int h);               //调整图片控件尺寸
	void setupPolygonRegion();                         //调整窗口外形
	void fullDlg();                                    //图片填充窗口
	void fullScreen(int width,int height);             //窗口按图片原始比例居中填充屏幕
	void setZoom(bool flag);                           //设置窗口可缩放
	bool getPicSize(CString name,int& w,int& h);       //读取图片尺寸

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnPaint();

public:
	CObject* parent;

private:
	CStatic m_picCtr;
	CString sPath;
	CImage img;
	int colorType;
	bool bZoom;
	int centerX;
	int centerY;

	bool isTransparent;
};
