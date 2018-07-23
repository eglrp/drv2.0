#pragma once
#include "comFunction.h"
#include "resource.h"
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "IViewer3D.h"
#include "DlgAttInfo.h"
#include <osgEarthSymbology/Geometry>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include "DrawAreaHandler.h"
#include "CMyListCtr.h"
USING_NAMESPACE_EARTHMATRIX

// CDlgSearchAdv 对话框

class CDlgSearchAdv : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSearchAdv)

public:
	CDlgSearchAdv(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSearchAdv();

// 对话框数据
	enum { IDD = IDD_DIALOG_SEARCH_ADV };

    void DoSearch();
    void DoSearchWithFilter();

	CString m_sKey;
	CString m_sField;
	CString m_sKey2;
	CString m_sField2;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDosearch();
	CComboBox m_cFields;
	CString m_sShp;
	CEdit m_cKey;
	CMyListCtrl m_cList;
	CObject* parent;
	x3::Object<IViewer3D> m_spViewer3D;

	osg::ref_ptr<osg::Geode> AddPolygonNode(osg::Vec3Array* arr,osg::Vec4 color);
	void MakeBuildingStruct(std::string sTH,std::string sZ,osg::Vec4 color = osg::Vec4(1,1,0,1));
	void MakeBuildingStructAll(std::string sTH,std::string sZ,osg::Group* group,osg::Vec4 color = osg::Vec4(1,1,0,1));
	double GetBuildingHeight();
	bool BuildingInThisHeight(double height, int & nCrossNum);
	void ShowAttWin();
	//bool allAlpha(std::string str);
	
	osg::ref_ptr<osg::Group> gTemp;
	osg::ref_ptr<osgEarth::Features::FeatureSource> features;
	osg::ref_ptr<osgEarth::Symbology::Geometry> m_geoPart;
	std::vector<osg::ref_ptr<osgEarth::Symbology::Geometry>> v;
	std::vector<osg::ref_ptr<osgEarth::Features::Feature>> vFeature;

	double dStep;
	double dStartHeight;
	double dEndHeight;
	bool bFindedStart;
	bool bFindedEnd;
	int aPtsInHeight[1024];
	CDlgAttInfo* m_pAttInfoWin;
	osg::ref_ptr<CvlMeasureToolHandler> m_pDrawAreaHandle;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	void MakeFloor3DBorder(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Vec4 color = osg::Vec4(1,1,0,1));
	void MakeFloor3DBorderAll(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Group* group,osg::Vec4 color = osg::Vec4(1,1,0,1));
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckShowAll();
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonSerchCircle();
	CButton m_combox_filter;
	CString m_lowValue;
	CString m_upValue;
	CComboBox m_combo_filter;
	CString m_numResults;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_dlgTitle;
	CButton m_EXIT;

	CBrush m_brBk;
	CFont m_font;
	CFont m_font1;
	CObject* parent1;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonSearchadvexit();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	CComboBox m_cFields2;
	CEdit m_cKey2;
	RECT _rect;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


class COsgGeodeFinder : public osg::NodeVisitor
{

public:
	COsgGeodeFinder() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) 
	{
		isFindGeometry = false;
		vec_ptr.clear();
	}

	virtual void apply(osg::Geode& geode)
	{
		isFindGeometry = true;                                            /////修改成保存
		traverse(geode);
		vec_ptr.push_back(&geode);
	}

public:
	bool isFindGeometry;

	std::vector<osg::Geode*> vec_ptr;
};