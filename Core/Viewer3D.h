#pragma once
#include "IViewer3D.h"
#include "osgViewer/Viewer"
#include "osgViewer/ViewerEventHandlers"
#include "osgGA/KeySwitchMatrixManipulator"
#include "osgGA/StateSetManipulator"
#include "osgText/Text"
#include "x3py/module/classmacro.h"
#include "x3py/utilfunc/lockrw.h"
#include "osgViewer/Viewer"
#include <osgEarthUtil/EarthManipulator>

#if defined(_MSC_VER) && _MSC_VER > 1200    // VC8/9
	#include <hash_map>
	using stdext::hash_multimap;
	using stdext::hash_map;
#else                                       // VC6, GCC or others
	#define hash_multimap std::multimap
	#define hash_map std::map
#endif

using namespace osgEarth;
using namespace osgEarth::Util;

BEGIN_NAMESPACE_EARTHMATRIX

class CViewerHandler : public osgGA::GUIEventHandler
{
public:
	CViewerHandler();
	virtual ~CViewerHandler();
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
	void setWindowSize(osg::Camera* pCamera, int width, int height);
};

class CViewer3D : public OpenThreads::Thread, public IViewer3D
{
	X3BEGIN_CLASS_DECLARE(CViewer3D, clsidViewer3D)		
		X3DEFINE_INTERFACE_ENTRY(IViewer3D)
	X3END_CLASS_DECLARE()

public:
	CViewer3D(void);
	~CViewer3D(void);

public:
	virtual bool Create(size_t hWnd, osg::Node* node = NULL, int x = 0, int y = 0, int width = 0, int height = 0, osg::Vec4f color = osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
	bool Create2D(size_t hWnd, osg::Node* node = NULL, int x = 0, int y = 0, int width = 0, int height = 0, osg::Vec4f color = osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f));
	virtual bool Destroy();

	virtual osgViewer::Viewer* getViewer() { return m_spViewer.get(); }
	virtual osg::Node* getRootNode() { if( m_spViewer==NULL ) return NULL; return m_spViewer->getSceneData(); }
	virtual osg::Node* getSceneData() { return m_spData.get(); }
	virtual MapNode* getMapNode() { return m_spMapNode.get(); }
	virtual EarthManipulator* getEarthManipulator() { return m_spEarthManipulator.get(); }

	virtual bool OpenFile(const std::string& file, osg::Node* pParentNode = NULL);
	virtual bool AddNode(osg::Node* pNode, osg::Node* pParentNode = NULL);
	virtual bool RemoveNode(osg::Node* pNode, osg::Node* pParentNode = NULL);
	virtual bool CenterNode(osg::Node* pNode, double dMinDis);
	virtual bool HighlightNode(osg::Node* pNode, bool bHighlight);
	virtual bool ChangePosition(int nWidth, int nHeight);
	virtual bool FlyToViewport(double x, double y, double z, double range = 2000);
	virtual std::string GetFileContent(std::string FilePath);
	virtual std::wstring RoundOffToString(double f, int n);
	virtual CStringArray* GetLayersArray(){ return m_aLayerFilePath; };
	virtual CStringArray* GetPathsArray(){ return m_aPathFilePath; };
	virtual CString GetProjectFilePath(){ return m_sProjectFilePath; };
	virtual CString GetDefLayer(){ return m_sDefLayer; };
	virtual void SetProjectFilePath(CString sPath);
	virtual void SetDefLayer(CString sPath);
	virtual void SetRecordPath(std::string sPath);
	virtual std::string GetRecordPath();
	virtual void SetOSG_MAX_PAGEDLOD(int nMax);
	virtual void SetOSG_NUM_DATABASE_THREADS(int nThreads);
	virtual void SetScreenNum(int nNum);
	virtual int GetOSG_MAX_PAGEDLOD(){ return m_nOSG_MAX_PAGEDLOD;};
	virtual int GetOSG_NUM_DATABASE_THREADS(){ return m_nOSG_NUM_DATABASE_THREADS;};
	virtual int GetScreenNum(){ return m_nScreenNum;};
	virtual CStringArray* GetLayerCheck(){ return m_aLayerCheck;};
	virtual CString GetViewportPos();
	virtual void SetViewport(CString s);
	virtual void SetPrj(std::string) ;
	virtual std::string GetPrj() ;
	virtual osgGA::StateSetManipulator* getStateSetManipulator() { return m_spStatesetManipulator.get(); }

public:	
	virtual void StartRender();
	virtual void StopRender();
	virtual void Lock();
	virtual void Unlock();

protected:
	osg::Matrix matrixListtoSingle(const osg::MatrixList &tmplist);

private:
	void InitManipulators(void);
	virtual void run();	
	virtual void frame(double simulationTime=USE_REFERENCE_TIME);
	OpenThreads::Mutex _mutex;
	bool _firstFrame;

protected:
	HWND m_hWnd;
	osg::ref_ptr<osgViewer::Viewer> m_spViewer;
	osg::ref_ptr<osg::Group> m_spData;
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> m_spKeyswitchManipulator;
	osg::ref_ptr<osgGA::StateSetManipulator> m_spStatesetManipulator;

	osg::ref_ptr<EarthManipulator> m_spEarthManipulator;
	osg::ref_ptr<MapNode> m_spMapNode;
	osg::ref_ptr<osgViewer::RecordCameraPathHandler> m_spPathHandler;

	std::string m_sWorkDir;
	CStringArray* m_aLayerFilePath;
	CStringArray* m_aLayerCheck;
	CStringArray* m_aPathFilePath;
	CString m_sProjectFilePath;
	CString m_sDefLayer;

	std::string m_sPath;

	int m_nOSG_MAX_PAGEDLOD;
	int m_nOSG_NUM_DATABASE_THREADS;
	int m_nScreenNum;

	std::string m_prjInfo;

	CString m_sHouseDefLayer;
	CString m_sBuildDefLayer;
};

END_NAMESPACE_EARTHMATRIX


