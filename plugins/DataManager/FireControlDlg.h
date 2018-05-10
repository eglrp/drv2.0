#pragma once
#include "afxwin.h"
#include "sniperToolHandle.h"
#include "BackgroundDlg.h"
#include "DLGNeighborSearch.h"
#include "ShowPictureDlg.h"
#include "DLGQrencode2.h"
#include "DlgAttInfo.h"
// CFireControlDlg 对话框

class CFireControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFireControlDlg)

public:
	CFireControlDlg(int H ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFireControlDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_firecontrol };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();

	CObject* parent;
	CButton m_jqdw;
	CButton m_fwdw;
	CButton m_fwewm;
	CButton m_ldljb;
	CButton m_xfsfb;

	void play();

	//场景点击坐标
	double px;
	double py;
	double pz;

	GetPosHandler* m_pGetPosHandler;
private:
	int viewH;

	bool checkPos();
protected:
	int nElapse;
	int count;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLdljb();
	afx_msg void OnBnClickedButtonJqdw();
	afx_msg void OnBnClickedButtonFwdw();
	afx_msg void OnBnClickedButtonFwewm();
	afx_msg void OnBnClickedButtonXfsfb();

	std::string getLocationStr(double lon,double lat,std::string str1 = "理想城定位点-航天远景",std::string str2 = "理想城一号楼");
	void PointToWGS84(double x, double y, double& lon, double& lat);
	void readPrjInfo();
	std::string replaceFileName(std::string srcStr, std::string replaceStr);
	std::string getHouseInfo(HOUSEINFO& gHouseInfo);

	//房间经纬度
	double lon;
	double lat;
	std::string prjInfo;
public:
	CBackgroundDlg* pDLG_BK;
	CShowPictureDlg* pDLG_PIC;
	CDLGNeighborSearch* pDLG_NEIBOR;
	CDLGQrencode2* pDLG_code;
	CDlgAttInfo* pDLG_INFO;

	void destroyAll();
};
