#pragma once
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
#include "osg/vec3d"
#include <vector>
#include "DLGCreatorData.h"

//一点对多点
struct CPtToMultiPt
{
	osg::Vec3d pt;
	std::vector<osg::Vec3d> vecMultiPt;
};

// CDLGBuildingVisiableSurvey dialog

class CDLGBuildingVisiableSurvey : public CDialogEx
{
	DECLARE_DYNAMIC(CDLGBuildingVisiableSurvey)

public:
	CDLGBuildingVisiableSurvey(CString sDefLayer,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDLGBuildingVisiableSurvey();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonLoadxml();
	afx_msg void OnCbnSelchangeComboQueryType();
	afx_msg void OnBnClickedButtonDosearch();

	void doSearch(CString sObj,CString sXml,int queryType);//执行搜索

public:
	//保存所有建筑名称
	std::vector<CString> m_vecAllBuilding;
	//保存所有观察点名称 用于填充combox控件值
	std::vector<CString> m_vecAllObservePt;
	//保存所有观察点坐标 用于绘制
	std::vector<osg::Vec3d> mVecObservePt;
	//保存搜索结果
	CPtToMultiPt mPtToMultiPt;

public:
	CObject* parent;
	CComboBox mComQueryType;
	CComboBox mComQueryObj;
	CListCtrl mWndList;
	CDLGCreatorData* p_mDLGDataCreator;	

protected:
	//默认矢量图层shp
	CString mDefLayer;
	//用于搜索的xml文件
	CString mXmlFile;
	//控制初始化时加载xml文件
	bool mbNoXml;

	bool isBusy;
public:
	CListCtrl mWndList_item;
	afx_msg void OnNMClickListQueryItem(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
