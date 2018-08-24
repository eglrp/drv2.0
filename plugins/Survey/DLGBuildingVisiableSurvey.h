#pragma once
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
#include "osg/vec3d"
#include <vector>
#include "DLGCreatorData.h"
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

	CObject* parent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CString mDefLayer;
	CString mXmlFile;
	bool mbNoXml;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonLoadxml();
	CComboBox mComQueryType;
	CComboBox mComQueryObj;

	std::vector<CString> m_vecAllBuilding;
	std::vector<CString> m_vecAllObservePt;
	afx_msg void OnCbnSelchangeComboQueryType();
	afx_msg void OnBnClickedButtonDosearch();

	void doSearch(CString sObj,CString sXml,int queryType);
	CListCtrl mWndList;
	virtual BOOL OnInitDialog();

	std::vector<osg::Vec3d> mVecObservePt;
	CPtToMultiPt mPtToMultiPt;

	CDLGCreatorData* p_mDLGDataCreator;
};
