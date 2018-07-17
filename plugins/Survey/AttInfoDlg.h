#pragma once
#include "Resource.h"
#include "CMyListCtr.h"
#include "DlgAttInfoEx.h"
#include "ShowPictureDlg.h"
#include "QrenCodeDlg.h"
#include <vector>

// CAttInfoDlg 对话框

class CAttInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAttInfoDlg)

public:
	CAttInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAttInfoDlg();

	CMyListCtrl	m_wndList;
	CButton m_EXIT;
// 对话框数据
	enum { IDD = IDD_DIALOG_POINTSELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	void InsertRow(CString sName, CString sValue);         //插入数据
	void RemoveAllRows();								   //清空数据
	void setupPolygonRegion(SHOWMODE mode);                //设置外形
	void MovetoPoint(RECT viewRect);                       //窗口移动到鼠标处
	void suitHeightAndWidth(int n,int maxW);               //根据list中的项数调整行高列宽
	void changeDlgHeight(int h);                           //调整窗口高
	void analysisString(std::string inS,std::vector<CString>& vec);//解析字符串
	void setLocation(double a,double b){lon = a;lat = b;}
	std::string getLocationStr(double lon,double lat,std::string str1 = "理想城定位点-航天远景",std::string str2 = "理想城一号楼");
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
	//房间经纬度
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

