#pragma once
#include "Resource.h"
#include "CMyListCtr.h"
#include "DlgAttInfoEx.h"
#include "ShowPictureDlg.h"
#include "QrenCodeDlg.h"
#include <vector>

// CAttInfoDlg �Ի���

class CAttInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAttInfoDlg)

public:
	CAttInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAttInfoDlg();

	CMyListCtrl	m_wndList;
	CButton m_EXIT;
// �Ի�������
	enum { IDD = IDD_DIALOG_POINTSELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	void InsertRow(CString sName, CString sValue);         //��������
	void RemoveAllRows();								   //�������
	void setupPolygonRegion(SHOWMODE mode);                //��������
	void MovetoPoint(RECT viewRect);                       //�����ƶ�����괦
	void suitHeightAndWidth(int n,int maxW);               //����list�е����������и��п�
	void changeDlgHeight(int h);                           //�������ڸ�
	void analysisString(std::string inS,std::vector<CString>& vec);//�����ַ���
	void setLocation(double a,double b){lon = a;lat = b;}
	std::string getLocationStr(double lon,double lat,std::string str1 = "����Ƕ�λ��-����Զ��",std::string str2 = "�����һ��¥");
	void readPrjInfo();
	void PointToWGS84(double x, double y, double& lon, double& lat);
	std::string replaceFileName(std::string srcStr, std::string replaceStr);
	std::string getHouseInfoStr();

	std::string prjInfo;
public:
	std::vector<CString> vName;
	std::vector<CString> vID;
	CObject* parent;
	CObject* parentBk;
	CQrenCodeDlg* pQren;
	CDlgAttInfoEx* pChild;
	CShowPictureDlg* pChild1;

private:
	CFont m_font;
	//���侭γ��
	double lon;
	double lat;
	

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMClickListAtt(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1Entiexit();
	void selfDefShowWindow(UINT i);

	int btnW;
	int btnH;
	RECT _rect;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

