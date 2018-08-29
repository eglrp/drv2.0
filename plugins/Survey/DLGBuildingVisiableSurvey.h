#pragma once
#include "resource.h"
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
#include "osg/vec3d"
#include <vector>
#include "DLGCreatorData.h"

//һ��Զ��
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

	void doSearch(CString sObj,CString sXml,int queryType);//ִ������

public:
	//�������н�������
	std::vector<CString> m_vecAllBuilding;
	//�������й۲������ �������combox�ؼ�ֵ
	std::vector<CString> m_vecAllObservePt;
	//�������й۲������ ���ڻ���
	std::vector<osg::Vec3d> mVecObservePt;
	//�����������
	CPtToMultiPt mPtToMultiPt;

public:
	CObject* parent;
	CComboBox mComQueryType;
	CComboBox mComQueryObj;
	CListCtrl mWndList;
	CDLGCreatorData* p_mDLGDataCreator;	

protected:
	//Ĭ��ʸ��ͼ��shp
	CString mDefLayer;
	//����������xml�ļ�
	CString mXmlFile;
	//���Ƴ�ʼ��ʱ����xml�ļ�
	bool mbNoXml;

	bool isBusy;
public:
	CListCtrl mWndList_item;
	afx_msg void OnNMClickListQueryItem(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
