#pragma once
#include "comFunction.h"
#include "UIPlugins.h"
#include "x3py/module/classmacro.h"
#include <afxtoolbar.h>
#include "osg/Node"
#include "TreeCtrlX.h"
#include "IViewer3D.h"
#include <osgEarthSymbology/Geometry>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include "DlgAttInfo.h"
#include "DlgSearchAdv.h"
#include "HtmlDlg.h"
#include "ShowPictureDlg.h"
#include "CVMediaPlayerDlg.h"
#include "InternetInfoDlg.h"
#include "DlgDB_Connect.h"
#include "DLGVLCPlayer.h"

USING_NAMESPACE_EARTHMATRIX

class CDataManagerToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{		
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CDataManagerObserver : public x3::ObserverObject
{
public:
	CDataManagerObserver();
	~CDataManagerObserver();

public:
	// Event
	void registerHandlers();
	void unregisterHandlers();	

	// Node Event
	virtual void OnNodeAdd(x3::IObject* pBuddy, osg::Node* pNode, osg::Node* pParentNode, std::string filename) = 0;
	virtual void OnNodeRemove(x3::IObject* pBuddy, osg::Node* pNode, osg::Node* pParentNode) = 0;
	virtual void OnNodeChange(x3::IObject* pBuddy, osg::Node* pNode) = 0;
	virtual void OnNodeSelected(x3::IObject* pBuddy, osg::Node* pNode) = 0;
	virtual void OnSetDefLayer2(std::string filename) = 0;
	virtual void OnProjectOpen(std::string filename) = 0;

private:
	bool m_bRegister;
};

// CDataManager
const char* const clsidDataManager = "00000000-1000-0000-0000-000000000000";
class CDataManager : public CWnd, public IUICommon, public IUIWnd, public CDataManagerObserver
{
	X3BEGIN_CLASS_DECLARE(CDataManager, clsidDataManager)		
		X3DEFINE_INTERFACE_ENTRY(IUICommon)		
		X3DEFINE_INTERFACE_ENTRY(IUIWnd)		
	X3END_CLASS_DECLARE()
	DECLARE_DYNAMIC(CDataManager)

public:
	CDataManager();
	virtual ~CDataManager();

public:
	// IUICommon
	virtual bool Initialize();
	virtual bool UnInitialize();
	virtual bool SetBuddy(x3::IObject* val);

	// IUIWnd
	virtual bool createWnd(size_t hParent, long id);
	virtual size_t getHandle() { return (size_t)GetSafeHwnd(); }
	virtual bool setParent(size_t hWnd);
	virtual DWORD getStyle() { return WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI; }
public:
	// Node Event
	void OnNodeAdd(x3::IObject* pBuddy, osg::Node* pNode, osg::Node* pParentNode, std::string filename);
	void OnNodeRemove(x3::IObject* pBuddy, osg::Node* pNode, osg::Node* pParentNode);
	void OnNodeChange(x3::IObject* pBuddy, osg::Node* pNode);
	void OnNodeSelected(x3::IObject* pBuddy, osg::Node* pNode);
	void OnSetDefLayer2(std::string filename);
	void OnProjectOpen(std::string filename);
	void SetChildCheck(HTREEITEM hTree,BOOL bCheck);
	void SetChildMask(HTREEITEM hTree,BOOL bCheck);
	void SetParentCheck(HTREEITEM hTree,BOOL bCheck);	void RemoveAllNode(HTREEITEM hTree);	void SetLayerCheck(CString sName, bool bCheck);	HTREEITEM FindItem(HTREEITEM item, CString strText);
	void ExpandAll(HTREEITEM hroot);
	

protected:
	void AdjustLayout();

public:
	afx_msg void OnBnClickedButtonSearchAdv();
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickDataManager(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSearch();
	

	DECLARE_MESSAGE_MAP()

public:
	CDataManagerToolBar m_wndToolBar;
	CTreeCtrlX m_wndTreeCtrl;
	CEdit m_cKey;
	CListCtrl m_cList;
	CButton m_cSearch;
	CButton m_cSearchAdv;
	CButton m_cShowSrch;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	afx_msg void OnRBClick(NMHDR* pNMHDR, LRESULT* pResult);    
	afx_msg void OnDellayer();
	afx_msg void OnSetDefLayer();
	afx_msg void OnKeyChangeEditName();
	CString FindSHPFile();
	CString FindDefSHPFile(CString m_sDefLayer);
	void DoSearch(CString sShpFile);
	void DoSearchKey(CString sShpFile,CString m_key);
	void FillFieldsToDlg(CString sShp);
	void MakeFloor3DBorder(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Vec4 color = osg::Vec4(1,1,0,1),bool showAtt = true);
	void MakeFloor3DBorderAll(osgEarth::Features::Feature* feature, double dFloor, double dCeil,osg::Group* group);
	void ListByKey(CString str);

	void UnBold();
	void cancelDefLayer(CString m_BuildDefLayer);
	CString m_sDefLayer;
	CString m_HouseDefLayer;
	CString m_BuildDefLayer;
	CString m_sKey;
	x3::Object<IViewer3D> m_spViewer3D;

	osg::ref_ptr<osg::Geode> AddPolygonNode(osg::Vec3Array* arr,osg::Vec4 color = osg::Vec4(1,1,0,1));
	void MakeBuildingStruct(std::string sTH,std::string sZ);
	void MakeBuildingStructAll(std::string sTH,std::string sZ,osg::Group* group);
	double GetBuildingHeight();
	bool BuildingInThisHeight(double height, int & nCrossNum);
	void ShowAttWin();
	osg::PositionAttitudeTransform* create3DText(const osg::Vec3& center,float radius, CString);
	osg::Group* createText(const osg::Vec3& center,float radius, CString);
	void ShowPOI(double , double, double, CString);
	void hideAll();
	

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
	CHtmlDlg* m_pHtmlInfoWin;
	CShowPictureDlg* m_pShowPictureInfoWin;
	CCVMediaPlayerDlg* m_pCVMediaPlayerInfoWin;
	CDlgSearchAdv* m_pSearchAdv;
	CInternetInfoDlg* m_pInternetInfoWin;
	CDLGVLCPlayer* m_pVLCPlayerInfoWin;

	bool bInterDel;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSetHouseLayer();
	afx_msg void OnSetBuildLayer();
};




class QMemDC :	// 我把双缓存封装到类中，这样就方便多了
	public CDC
{
private:
	CDC* dcSrc;
	CRect rect;
	CBitmap bmp;
public:
	QMemDC(CDC* dc,CRect rc);
	void Apply();
};