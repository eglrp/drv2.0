#pragma once
#include "resource.h"
#include "afxwin.h"

// CShowPictureDlg �Ի���

class CShowPictureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShowPictureDlg)

public:
	CShowPictureDlg(bool isTranparent = true,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowPictureDlg();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PICTUREINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:	
	void setPath(std::string path);                    //����ͼƬ·��
	void setColorType(int c);                          //���ô��ڱ�����ɫ
	void changePictureSize(int w,int h);               //����ͼƬ�ؼ��ߴ�
	void setupPolygonRegion();                         //������������
	void fullDlg();                                    //ͼƬ��䴰��
	void fullScreen(int width,int height);             //���ڰ�ͼƬԭʼ�������������Ļ
	void setZoom(bool flag);                           //���ô��ڿ�����
	bool getPicSize(CString name,int& w,int& h);       //��ȡͼƬ�ߴ�

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
