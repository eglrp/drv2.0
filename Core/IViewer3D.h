#pragma once
#include "EarthMatrixExports.h"
#include "x3py/module/plugininc.h"
#include "x3py/objptr.h"
#include <string>
#include "osg/Node"
#include "osgViewer/Viewer"
#include "osgGA/StateSetManipulator"
#include <osgEarthUtil/EarthManipulator>
#include "osgEarth/MapNode"

BEGIN_NAMESPACE_EARTHMATRIX

using namespace osgEarth;
using namespace osgEarth::Util;

const char* const clsidViewer3D = "A39EF1C6-379F-4B63-984B-D68A18426B55";
class IViewer3D : public x3::IObject
{
	X3DEFINE_IID(IViewer3D);

public:
	virtual bool Create(size_t hWnd, osg::Node* node = nullptr, int x = 0, int y = 0, int width = 0, int height = 0, osg::Vec4f color = osg::Vec4f(0.2f, 0.2f, 0.4f, 1.0f)) = 0;
	virtual bool Destroy() = 0;

public:
	// Viewer
	virtual osgViewer::Viewer* getViewer() = 0;
	virtual osg::Node* getRootNode() = 0;
	virtual osg::Node* getSceneData() = 0;
	virtual osgEarth::MapNode* getMapNode() = 0;
	virtual EarthManipulator* getEarthManipulator() = 0;

	virtual osgGA::StateSetManipulator* getStateSetManipulator() = 0;

	// Node 
	virtual bool OpenFile(const std::string& file, osg::Node* pParentNode = nullptr) = 0;
	virtual bool ChangePosition(int nWidth, int nHeight) = 0;
	virtual bool AddNode(osg::Node* pNode, osg::Node* pParentNode = nullptr) = 0;
	virtual bool RemoveNode(osg::Node* pNode, osg::Node* pParentNode = nullptr) = 0;

	virtual bool CenterNode(osg::Node* pNode, double dMinDis = 10.0) = 0;
	virtual bool HighlightNode(osg::Node* pNode, bool bHighlight) = 0;
	virtual bool FlyToViewport(double x, double y, double z, double range = 2000) = 0;
	virtual std::string GetFileContent(std::string FilePath) = 0;
	virtual std::wstring RoundOffToString(double f, int n) = 0;
	virtual CStringArray* GetLayersArray() = 0;
	virtual CString GetProjectFilePath() = 0;
	virtual CStringArray* GetPathsArray() = 0;
	virtual void SetProjectFilePath(CString sPath) = 0;
	virtual void SetRecordPath(std::string sPath) = 0;
	virtual std::string GetRecordPath() = 0;
	virtual void SetOSG_MAX_PAGEDLOD(int nMax) = 0;
	virtual int GetOSG_MAX_PAGEDLOD() = 0;
	virtual void SetOSG_NUM_DATABASE_THREADS(int nMax) = 0;
	virtual int GetOSG_NUM_DATABASE_THREADS() = 0;
	virtual void SetScreenNum(int nNum) = 0;
	virtual int GetScreenNum() = 0;
	virtual CString GetDefLayer() = 0;
	virtual void SetDefLayer(CString sPath) = 0;
	virtual CStringArray* GetLayerCheck() = 0;
	virtual CString GetViewportPos() = 0;
	virtual void SetViewport(CString s) = 0;
	virtual void SetPrj(std::string) = 0;
	virtual std::string GetPrj() = 0;
	/*virtual void SetHouseDefLayer(CString s) = 0;
	virtual void SetBuildDefLayer(CString s) = 0;
	virtual CString GetHouseDefLayer() = 0;
	virtual CString GetBuildDefLayer() = 0;*/

public:	
	virtual void StartRender() = 0;
	virtual void StopRender() = 0;
	virtual void Lock() = 0;
	virtual void Unlock() = 0;


	RECT rectView3D;
};

END_NAMESPACE_EARTHMATRIX