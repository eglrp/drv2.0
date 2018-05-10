#pragma once
#include "resource.h"
#include "CvvImage.h"
#include "cv.h"
#include "highgui.h"
using namespace cv;
// CCVMediaPlayerDlg 对话框

class CCVMediaPlayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCVMediaPlayerDlg)

public:
	CCVMediaPlayerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCVMediaPlayerDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CVPLAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void DrawPicToHDC(IplImage *img, UINT ID);
	void changePlayerSize(int w,int h);
	void setupPolygonRegion();
	bool open(std::string s)
	{
		filePath = s;
		capture.release();
		bool t = capture.open(filePath.c_str());
		if (t)
		{
			SetTimer(1,20,NULL);
		}
		return t;
	}

	int rect_height;
	int rect_width;

	RECT rect;
	VideoCapture capture;
	std::string filePath;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	CString m_gh;
	CString m_gspq;
	CString m_sblx;
	CString m_sbcs;
	CString m_whry;
	CString m_whtel;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CFont m_font;
};
