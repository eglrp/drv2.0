#include "stdafx.h"
#include "Viewer3D.h"
#include "INodeEvents.h"
#include "osg/StateSet"
#include "osgDB/ImagePager"
#include "osgDB/Registry"
#include "osgDB/ReadFile"
#include "osgViewer/api/win32/GraphicsWindowWin32"

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/SphericalManipulator>
#include "SelfDefTerrainManipulator.h"
#include "CSelfDefKeySwitchMatrixManipulator.h"

#include "EarthMatrixFunctions.h"
#include <math.h>
#include "ThreadSafeUpdateCallback.h"
#include "ImagePagerEx.h"
#include "x3py/manager/iworkpath.h"

//////////////////////////////郑思成////////////////////////////////////////////

#include <osgEarthUtil/Sky>
#include <osgEarthUtil/LODBlending>
#include <osgEarthUtil/VerticalScale>
#include <osgEarthUtil/ContourMap>
#include <osgEarthDrivers/model_simple/SimpleModelOptions>
#include "osgDB/FileNameUtils"
#include "osgDB/FileUtils"
//从2.7的源码中复制过来的
#include "osgEarth/Extension"

#include <ComFun.h>
#include "osg/AnimationPath"
using namespace osgEarth;
using namespace osgEarth::Util;
using namespace std;

//////////////////////////////郑思成////////////////////////////////////////////
BEGIN_NAMESPACE_EARTHMATRIX

//////////////////////////////////////////////////////////////////////////
// CViewerHandler
CViewerHandler::CViewerHandler()
{
}

CViewerHandler::~CViewerHandler()
{
}

bool CViewerHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{

    osg::ref_ptr<osgViewer::View> spView = dynamic_cast<osgViewer::View*>(&aa);
    if( spView == NULL || spView->getCamera() == NULL )
        return false;

    switch(ea.getEventType())
    {
    case osgGA::GUIEventAdapter::RESIZE:
    {
        setWindowSize(spView->getCamera(), ea.getWindowWidth(), ea.getWindowHeight());
    }
    break;

    default:
        break;
    }
    return false;
}

void CViewerHandler::setWindowSize(osg::Camera* pCamera, int width, int height)
{
    if( pCamera == NULL || width <= 0 || height <= 0 )
        return;

    pCamera->setViewport(0, 0, width, height);

    double fovy;
    double aspectRatio;
    double zNear;
    double zFar;
    if( pCamera->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar) )
        pCamera->setProjectionMatrixAsPerspective(fovy, (double)width / (double)height, zNear, zFar);
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
CViewer3D::CViewer3D(void)
{
    m_hWnd = NULL;
    m_spViewer = NULL;
    m_spData = NULL;
    m_spKeyswitchManipulator = NULL;
    m_spStatesetManipulator = NULL;
    _firstFrame = true;

    x3::Object<x3::IAppWorkPath> spAppWorkPath(x3::clsidManager);
    std::wstring path = spAppWorkPath->getWorkPath();
    m_sWorkDir = x3::w2a(path);
    //osgDB::Registry::instance()->setBuildKdTreesHint(osgDB::ReaderWriter::Options::BUILD_KDTREES);
    //osgDB::Registry::instance()->getOrCreateSharedStateManager();

    m_aLayerFilePath = new CStringArray();
	m_aPathFilePath = new CStringArray();
	m_aLayerCheck = new CStringArray();
	m_spPathHandler = nullptr;

	m_nOSG_MAX_PAGEDLOD = 300;
	m_nOSG_NUM_DATABASE_THREADS = 4;
	m_nScreenNum = 1;

	m_sDefLayer = _T("");
}

CViewer3D::~CViewer3D(void)
{
    Destroy();
}

bool CViewer3D::ChangePosition(int nWidth, int nHeight)
{
    /*
    #ifdef _2D
    #else
    	Lock();
    	compass->ChangePosition(nWidth, nHeight);
    	Unlock();
    #endif

        return true;
    	*/
    return true;
}

void CViewer3D::InitManipulators(void)
{
    if( m_spKeyswitchManipulator != NULL )
        return;

    // Create a Manipulator Switcher
    //m_spKeyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;
	m_spKeyswitchManipulator = new CSelfDefKeySwitchMatrixManipulator;

#ifdef _2D
    //osg::ref_ptr<osgGA::CameraManipulator> defaultManipulator = new osgGA::TerrainManipulator();
	osg::ref_ptr<osgGA::CameraManipulator> defaultManipulator = new CSelfDefTerrainManipulator();
#else
    osg::ref_ptr<EarthManipulator> defaultManipulator = new EarthManipulator();
    m_spEarthManipulator = defaultManipulator;
#endif


    // Add our trackball manipulator to the switcher
    m_spKeyswitchManipulator->addMatrixManipulator( 0, "Default", defaultManipulator.get());
#ifdef _2D
	m_spKeyswitchManipulator->addMatrixManipulator( 1, "Trackball", new osgGA::TrackballManipulator() );
	m_spKeyswitchManipulator->addMatrixManipulator( 2, "Flight", new osgGA::FlightManipulator() );
	m_spKeyswitchManipulator->addMatrixManipulator( 3, "Drive", new osgGA::DriveManipulator() );
	m_spKeyswitchManipulator->addMatrixManipulator( 4, "Terrain", new osgGA::TerrainManipulator() );
	m_spKeyswitchManipulator->addMatrixManipulator( 5, "Orbit", new osgGA::OrbitManipulator() );
	m_spKeyswitchManipulator->addMatrixManipulator( 6, "FirstPerson", new osgGA::FirstPersonManipulator() );
	m_spKeyswitchManipulator->addMatrixManipulator( 7, "Spherical", new osgGA::SphericalManipulator() );
#else

#endif


    // Init the switcher to the first manipulator (in this case the only manipulator)
    m_spKeyswitchManipulator->selectMatrixManipulator(0);  // Zero based index Value
}

bool CViewer3D::Create(size_t hWnd, osg::Node* node, int x, int y, int width, int height, osg::Vec4f color)
{
#ifdef _2D
    Create2D(hWnd, node, x , y, width, height, color);
#else
    USES_CONVERSION;

    if( (HWND)hWnd == nullptr )
        return false;

    Destroy();

    InitManipulators();

    m_hWnd = (HWND)hWnd;
    // Local Variable to hold window size data
    RECT rect;

    // Create the viewer for this window
    m_spViewer = new osgViewer::Viewer();

    //m_spViewer->setImagePager(new ImagePagerEx());
    m_spViewer->addEventHandler(new CViewerHandler);
    // Add a Stats Handler to the viewer
    //#ifdef _DEBUG
    m_spViewer->addEventHandler(new osgViewer::StatsHandler);
    //#endif
    // 		//	m_pViewer->addEventHandler(new osgViewer::WindowSizeHandler);
    // 		//	m_pViewer->addEventHandler(new osgViewer::HelpHandler);
    // 		// 	m_pViewer->addEventHandler(new osgViewer::LODScaleHandler);
    // 		// 	m_pViewer->addEventHandler(new osgViewer::RecordCameraPathHandler);
    // 		// 	m_pViewer->addEventHandler(new osgViewer::ScreenCaptureHandler);
    // 		// 	m_pViewer->addEventHandler(new osgViewer::ToggleSyncToVBlankHandler);
    // 		// 	m_pViewer->addEventHandler(new osgViewer::ThreadingHandler);
    //
    // 		m_spPickModelHandler = new CPickModelHandler;
    // 		m_spPickModelHandler->SetHwnd(hWnd);
    // 		m_pViewer->addEventHandler(m_spPickModelHandler.get());
    //
    // 		m_spPlaceModelHandler = new PlaceModelHandler;
    // 		m_pViewer->addEventHandler(m_spPlaceModelHandler.get());
    //

    m_spViewer->addEventHandler(new osgViewer::LODScaleHandler);
    m_spViewer->addEventHandler(new osgViewer::ThreadingHandler);
    m_spViewer->addEventHandler(new osgGA::StateSetManipulator(m_spViewer->getCamera()->getOrCreateStateSet()));
    m_spViewer->addEventHandler(new osgViewer::RecordCameraPathHandler);
    //m_spViewer->addEventHandler(new FlyViewPointHandler);

    if( width == 0 || height == 0 )
    {
        // Get the current window size
        ::GetWindowRect(m_hWnd, &rect);
        // 初始窗体大小不能为0
        //if( rect.right==rect.left )
        width = GetSystemMetrics(SM_CXSCREEN) + 2 * ::GetSystemMetrics(SM_CXBORDER);
        //else
        //width = rect.right - rect.left;

        //if( rect.top==rect.bottom )
        height = GetSystemMetrics(SM_CYSCREEN) + 2 * ::GetSystemMetrics(SM_CYBORDER);
        //else
        //height = rect.bottom - rect.top;
    }


    // Init the GraphicsContext Traits
    osg::ref_ptr<osg::GraphicsContext::Traits> spTraits = new osg::GraphicsContext::Traits;

    // Setup the traits parameters
    spTraits->x = x;
    spTraits->y = y;
    spTraits->width = width;
    spTraits->height = height;
    spTraits->windowDecoration = false;
    spTraits->doubleBuffer = true;
    spTraits->sharedContext = 0;
    spTraits->setInheritedWindowPixelFormat = true;

    // Init the Windata Variable that holds the handle for the Window to display OSG in.
    spTraits->inheritedWindowData = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);
    //spTraits->quadBufferStereo = false;

    spTraits->alpha = osg::DisplaySettings::instance()->getMinimumNumAlphaBits();
    spTraits->stencil = osg::DisplaySettings::instance()->getMinimumNumStencilBits();
    spTraits->sampleBuffers = osg::DisplaySettings::instance()->getNumMultiSamples();
    spTraits->samples = osg::DisplaySettings::instance()->getNumMultiSamples();

    // Create the Graphics Context
    osg::ref_ptr<osg::GraphicsContext> spGraphicsContext = osg::GraphicsContext::createGraphicsContext(spTraits.get());

    // Init a new Camera (Master for this View)
    osg::ref_ptr<osg::Camera> spCamera = new osg::Camera;

    // Assign Graphics Context to the Camera
    spCamera->setGraphicsContext(spGraphicsContext);

    // Set the viewport for the Camera
    spCamera->setViewport(new osg::Viewport(0, 0, spTraits->width, spTraits->height));

    // Set projection matrix and camera attribtues
    //camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    //spCamera->setClearColor(color);
    //spCamera->setProjectionMatrixAsPerspective(
    //	60.0f, static_cast<double>(spTraits->width) / static_cast<double>(spTraits->height), 1.0f, 10000.0f);
    //spCamera->setNearFarRatio( 0.0005 );
    //spCamera->setSmallFeatureCullingPixelSize(0);
    //spCamera->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
    //spCamera->setNearFarRatio(0.00001f);

    spCamera->setDepthSortImpostorSprites(true);
    //spCamera->setCullingMode(osg::Camera::ENABLE_ALL_CULLING);
    spCamera->setProjectionMatrixAsPerspective( 60.0f, width / height, 0.5f, 10000.0f);

    spCamera->setClearColor(color);

    osg::ref_ptr<osg::StateSet> spStateset = spCamera->getOrCreateStateSet();
    spStateset->setGlobalDefaults();

    m_spStatesetManipulator = new osgGA::StateSetManipulator( spStateset.get() );
    m_spStatesetManipulator->setKeyEventCyclePolygonMode(-1);
    m_spStatesetManipulator->setKeyEventToggleBackfaceCulling(-1);
    m_spStatesetManipulator->setKeyEventToggleLighting(-1);
    m_spStatesetManipulator->setKeyEventToggleTexturing(-1);
    m_spStatesetManipulator->setBackfaceEnabled(false);
    m_spStatesetManipulator->setTextureEnabled(true);
    m_spStatesetManipulator->setLightingEnabled(true);

    // Add the Camera to the Viewer
    //m_pViewer->addSlave(camera.get());
    m_spViewer->setCamera(spCamera.get());

    // Add the Camera Manipulator to the Viewer
    m_spViewer->setCameraManipulator(m_spKeyswitchManipulator.get());

    //解决裁剪问题
    m_spViewer->getCamera()->setNearFarRatio(0.0000001f);
    m_spViewer->getCamera()->setSmallFeatureCullingPixelSize(-1);
    //m_spViewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);

    //////////////////////////////////////////////////////////////////////////
    if( m_spData == NULL )
    {
        osg::ref_ptr<osgDB::Options> options = new osgDB::Options();

        m_spData = new osg::Group();
        m_spData->addUpdateCallback(new CThreadSafeUpdateCallback);
        m_spData->setName(GETSTRINGA("CbersCore", "ROOTNAME", "数据容器"));

        osg::ref_ptr<osg::Node> nodefile = new osg::Node();

        x3::Object<x3::IAppWorkPath> spAppWorkPath(x3::clsidManager);
        std::wstring path = spAppWorkPath->getWorkPath();

        std::string earthPath = x3::w2a(path) + "Default.earth";
        nodefile = osgDB::readNodeFile( earthPath, options );

        m_spMapNode = MapNode::get(nodefile);

        m_spData->addChild(m_spMapNode);

        //////////////////////////////////////////////////////////////////////////
        const Config& externals = m_spMapNode->externalConfig();
        const Config& skyConf   = externals.child("sky");
        if ( !skyConf.empty() )
        {
            SkyOptions options(skyConf);
            if ( options.getDriver().empty() )
            {
                if ( m_spMapNode->getMapSRS()->isGeographic() )
                    options.setDriver("simple");
                else
                    options.setDriver("gl");
            }

            SkyNode* sky = SkyNode::create(options, m_spMapNode);
            if ( sky )
            {
                sky->attach( m_spViewer.get(), 0 );
                if ( m_spMapNode->getNumParents() > 0 )
                {
                    osgEarth::insertGroup(sky, m_spMapNode->getParent(0));
                }
                else
                {
                    sky->addChild( m_spMapNode );
                    m_spData = sky;
                }

                //sky->setMinimumAmbient(osg::Vec4(0,0,0,1));

            }
        }

        m_spMapNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

        const Config& lodBlendingConf = externals.child("lod_blending");
        const Config& vertScaleConf   = externals.child("vertical_scale");
        const Config& contourMapConf  = externals.child("contour_map");

        // Install elevation morphing
        if ( !lodBlendingConf.empty() )
        {
            m_spMapNode->getTerrainEngine()->addEffect( new LODBlending(lodBlendingConf) );
        }

        // Install vertical scaler
        if ( !vertScaleConf.empty() )
        {
            m_spMapNode->getTerrainEngine()->addEffect( new VerticalScale(vertScaleConf) );
        }

        // Install a contour map effect.
        if ( !contourMapConf.empty() )
        {
            m_spMapNode->getTerrainEngine()->addEffect( new ContourMap(contourMapConf) );
        }
    }

    // Process extensions.
    for(std::vector<osg::ref_ptr<Extension> >::const_iterator eiter = m_spMapNode->getExtensions().begin();
            eiter != m_spMapNode->getExtensions().end();
            ++eiter)
    {
        Extension* e = eiter->get();

        // Check for a View interface:
        ExtensionInterface<osg::View>* viewIF = ExtensionInterface<osg::View>::get( e );
        if ( viewIF )
            viewIF->connect( m_spViewer );
    }

    // Set the Scene Data
    m_spViewer->setSceneData(m_spData.get());



    // Realize the Viewer
    m_spViewer->realize();

    _firstFrame = true;
    StartRender();
#endif

    m_spViewer->setKeyEventSetsDone(0);

    return true;
}

bool CViewer3D::Create2D(size_t hWnd, osg::Node* node, int x, int y, int width, int height, osg::Vec4f color)
{
    if( hWnd == NULL )
        return false;

    Destroy();

    InitManipulators();

    m_spData = nullptr;

    osg::ref_ptr<osg::Group> spGroup = new osg::Group();
    spGroup->addUpdateCallback(new CThreadSafeUpdateCallback);
    spGroup->setName(GETSTRINGA(getClassName(), "ROOTNAME", "数据容器"));
    m_spData = spGroup;

    m_hWnd = (HWND)hWnd;
    // Local Variable to hold window size data
    RECT rect;

    // Create the viewer for this window
    m_spViewer = new osgViewer::Viewer();

    //m_spViewer->setImagePager(new ImagePagerEx());
    m_spViewer->addEventHandler(new CViewerHandler);
    // Add a Stats Handler to the viewer
    //#ifdef _DEBUG
    m_spViewer->addEventHandler(new osgViewer::StatsHandler);
    //#endif
    // 		//	m_pViewer->addEventHandler(new osgViewer::WindowSizeHandler);
    // 		//	m_pViewer->addEventHandler(new osgViewer::HelpHandler);
    // 		// 	m_pViewer->addEventHandler(new osgViewer::LODScaleHandler);
    // 		// 	m_pViewer->addEventHandler(new osgViewer::RecordCameraPathHandler);
    // 		// 	m_pViewer->addEventHandler(new osgViewer::ScreenCaptureHandler);
    // 		// 	m_pViewer->addEventHandler(new osgViewer::ToggleSyncToVBlankHandler);
    // 		// 	m_pViewer->addEventHandler(new osgViewer::ThreadingHandler);
    //
    // 		m_spPickModelHandler = new CPickModelHandler;
    // 		m_spPickModelHandler->SetHwnd(hWnd);
    // 		m_pViewer->addEventHandler(m_spPickModelHandler.get());
    //
    // 		m_spPlaceModelHandler = new PlaceModelHandler;
    // 		m_pViewer->addEventHandler(m_spPlaceModelHandler.get());
    //
    if( width == 0 || height == 0 )
    {
        // Get the current window size
        ::GetWindowRect(m_hWnd, &rect);
        // 初始窗体大小不能为0
        //if( rect.right==rect.left )
        width = GetSystemMetrics(SM_CXSCREEN) + 2 * ::GetSystemMetrics(SM_CXBORDER);
        //else
        //width = rect.right - rect.left;

        //if( rect.top==rect.bottom )
        height = GetSystemMetrics(SM_CYSCREEN) + 2 * ::GetSystemMetrics(SM_CYBORDER);
        //else
        //height = rect.bottom - rect.top;
    }


    // Init the GraphicsContext Traits
    osg::ref_ptr<osg::GraphicsContext::Traits> spTraits = new osg::GraphicsContext::Traits;

    // Setup the traits parameters
    spTraits->x = x;
    spTraits->y = y;
    spTraits->width = width;
    spTraits->height = height;
    spTraits->windowDecoration = false;
    spTraits->doubleBuffer = true;
    spTraits->sharedContext = 0;
    spTraits->setInheritedWindowPixelFormat = true;

    // Init the Windata Variable that holds the handle for the Window to display OSG in.
    spTraits->inheritedWindowData = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);
    //spTraits->quadBufferStereo = false;

    spTraits->alpha = osg::DisplaySettings::instance()->getMinimumNumAlphaBits();
    spTraits->stencil = osg::DisplaySettings::instance()->getMinimumNumStencilBits();
    spTraits->sampleBuffers = osg::DisplaySettings::instance()->getNumMultiSamples();
    spTraits->samples = osg::DisplaySettings::instance()->getNumMultiSamples();

    // Create the Graphics Context
    osg::ref_ptr<osg::GraphicsContext> spGraphicsContext = osg::GraphicsContext::createGraphicsContext(spTraits.get());

    // Init a new Camera (Master for this View)
    osg::ref_ptr<osg::Camera> spCamera = new osg::Camera;

    // Assign Graphics Context to the Camera
    spCamera->setGraphicsContext(spGraphicsContext);

    // Set the viewport for the Camera
    spCamera->setViewport(new osg::Viewport(0, 0, spTraits->width, spTraits->height));

    // Set projection matrix and camera attribtues
    //camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    //camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
    //spCamera->setClearColor(color);
    //spCamera->setProjectionMatrixAsPerspective(
    //	60.0f, static_cast<double>(spTraits->width) / static_cast<double>(spTraits->height), 1.0f, 10000.0f);
    //spCamera->setNearFarRatio( 0.0005 );
    //spCamera->setSmallFeatureCullingPixelSize(0);
    //spCamera->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
    //spCamera->setNearFarRatio(0.00001f);

    spCamera->setDepthSortImpostorSprites(true);
    //spCamera->setCullingMode(osg::Camera::ENABLE_ALL_CULLING);
    spCamera->setProjectionMatrixAsPerspective( 60.0f, width / height, 0.5f, 10000.0f);

    spCamera->setClearColor(color);

    osg::ref_ptr<osg::StateSet> spStateset = spCamera->getOrCreateStateSet();
    spStateset->setGlobalDefaults();

    m_spStatesetManipulator  = new osgGA::StateSetManipulator( spStateset.get() );
    m_spStatesetManipulator->setKeyEventCyclePolygonMode(-1);
    m_spStatesetManipulator->setKeyEventToggleBackfaceCulling(-1);
    m_spStatesetManipulator->setKeyEventToggleLighting(-1);
    m_spStatesetManipulator->setKeyEventToggleTexturing(-1);
    m_spStatesetManipulator->setBackfaceEnabled(false);
    m_spStatesetManipulator->setTextureEnabled(true);
    m_spStatesetManipulator->setLightingEnabled(false);

    // Add the Camera to the Viewer
    //m_pViewer->addSlave(camera.get());
    m_spViewer->setCamera(spCamera.get());

    // Add the Camera Manipulator to the Viewer
    m_spViewer->setCameraManipulator(m_spKeyswitchManipulator.get());

    // Set the Scene Data
    m_spViewer->setSceneData(m_spData.get());

    // Realize the Viewer
    m_spViewer->realize();

    _firstFrame = true;
    StartRender();
    return true;
}

bool CViewer3D::Destroy()
{
    StopRender();

    if( m_spViewer != NULL )
    {
        m_spViewer->setDone(true);
        m_spViewer->stopThreading();
    }

    m_spStatesetManipulator = NULL;
    m_spKeyswitchManipulator = NULL;
    m_spViewer = NULL;
    m_spData = NULL;
    m_hWnd = NULL;
    return true;
}

void CViewer3D::StartRender()
{
    StopRender();

    if( m_spViewer != NULL )
    {
        m_spViewer->setDone(false);
        start();
    }
}

void CViewer3D::StopRender()
{
    if( !isRunning() )
        return;

    cancel();
    if( m_spViewer )
        m_spViewer->setDone(true);

    while( isRunning() )
        OpenThreads::Thread::YieldCurrentThread();
}

void CViewer3D::Lock()
{
    _mutex.lock();
}

void CViewer3D::Unlock()
{
    _mutex.unlock();
}

void CViewer3D::run()
{
    if( m_spViewer == NULL )
        return;

    putenv("OSG_NUM_DATABASE_THREADS=4");
    //putenv("OSG_MAX_PAGEDLOD=5000");
    putenv("OSG_NUM_HTTP_DATABASE_THREADS=0");

    if (!m_spViewer->getCameraManipulator() && m_spViewer->getCamera()->getAllowEventFocus())
    {
        m_spViewer->setCameraManipulator(new osgGA::TrackballManipulator());
    }

    m_spViewer->setReleaseContextAtEndOfFrameHint(false);

    if (!m_spViewer->isRealized())
    {
        m_spViewer->realize();
    }

    const char* run_frame_count_str = getenv("OSG_RUN_FRAME_COUNT");
    unsigned int runTillFrameNumber = run_frame_count_str == 0 ? osg::UNINITIALIZED_FRAME_NUMBER : atoi(run_frame_count_str);

    while(!testCancel() && !m_spViewer->done() && (run_frame_count_str == 0 || m_spViewer->getViewerFrameStamp()->getFrameNumber() < runTillFrameNumber))
    {
        double minFrameTime = m_spViewer->getRunMaxFrameRate() > 0.0 ? 1.0 / m_spViewer->getRunMaxFrameRate() : 0.0;
        osg::Timer_t startFrameTick = osg::Timer::instance()->tick();

        if( testCancel() || m_spViewer->done() )
            break;

        if (m_spViewer->getRunFrameScheme() == osgViewer::ViewerBase::ON_DEMAND)
        {
            if (m_spViewer->checkNeedToDoFrame())
            {
                frame();
            }
            else
            {
                // we don't need to render a frame but we don't want to spin the run loop so make sure the minimum
                // loop time is 1/100th of second, if not otherwise set, so enabling the frame microSleep below to
                // avoid consume excessive CPU resources.
                if (minFrameTime == 0.0) minFrameTime = 0.01;
            }
        }
        else
        {
            frame();
        }

        if( testCancel() || m_spViewer->done() )
            break;

        // work out if we need to force a sleep to hold back the frame rate
        osg::Timer_t endFrameTick = osg::Timer::instance()->tick();
        double frameTime = osg::Timer::instance()->delta_s(startFrameTick, endFrameTick);
        if (frameTime < minFrameTime)
            OpenThreads::Thread::microSleep(static_cast<unsigned int>(1000000.0 * (minFrameTime - frameTime)));
    }
}

void CViewer3D::frame(double simulationTime)
{
    if( m_spViewer == NULL )
        return;

    if (m_spViewer->done())
        return;

    if (_firstFrame)
    {
        m_spViewer->init();

        if (!m_spViewer->isRealized())
        {
            m_spViewer->realize();
        }

        _firstFrame = false;
    }
    m_spViewer->advance(simulationTime);

    Lock();
    m_spViewer->eventTraversal();
    m_spViewer->updateTraversal();
    m_spViewer->renderingTraversals();
    Unlock();
}

osg::Matrix CViewer3D::matrixListtoSingle(const osg::MatrixList &tmplist)
{
    osg::Matrix tmp;

    if (tmplist.size() > 0)
    {
        unsigned int i;
        for (i = 1, tmp = tmplist[0]; i < tmplist.size(); i++)
            tmp *= tmplist[0];
        tmp = tmplist[0];
    }
    return (tmp);
}

bool CViewer3D::OpenFile(const std::string& filename, osg::Node* pParentNode)
{
    std::string FilePath = filename.substr( 0, filename.find_last_of( '\\' ) );
    std::string FileNameEx = filename.substr(filename.find_last_of( '\\' ) + 1);
    std::string FileNameNoEx = FileNameEx.substr(0, FileNameEx.find_last_of( '.' ));

    //setlocale( LC_ALL, "chs" );

    osg::ref_ptr<osg::Node> spNode = osgDB::readNodeFile(filename);

    //setlocale( LC_ALL, "C" );

    spNode->setName(FileNameNoEx);

    if( !spNode.valid() )
    {
        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    const osg::BoundingSphere& bs = spNode->getBound();
    if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
    {
        return false;
    }

    osg::Vec3d v3f = bs.center();
    osg::Vec3d v3dOut;

    const SpatialReference* epsg2382;
    std::string ProjFile = FilePath + "\\" + FileNameNoEx + ".prj";

    if (osgDB::fileExists(ProjFile))
    {
        std::string sContent = GetFileContent(ProjFile);
        epsg2382 = SpatialReference::create(sContent);

    }
    else
        epsg2382 = SpatialReference::create("wgs84");
    const SpatialReference* wgs84 = getMapNode()->getMapSRS();
    epsg2382->transform(v3f, wgs84, v3dOut);

    osg::Vec3d location = osg::Vec3d(v3dOut.x(), v3dOut.y(), v3dOut.z());

    //////////////////////////////////////////////////////////////////////////
    osgEarth::Drivers::SimpleModelOptions simpleModelOptions;
    simpleModelOptions.url() = osgEarth::URI(filename);
    simpleModelOptions.location() = location;
    simpleModelOptions.node() = spNode.get();
    osgEarth::ModelLayerOptions layerOptions(FileNameNoEx, simpleModelOptions);
    osg::ref_ptr<osgEarth::ModelLayer> mLayer = new osgEarth::ModelLayer(layerOptions);

    Lock();

    getMapNode()->getMap()->addModelLayer(mLayer.get());

    Unlock();
    //////////////////////////////////////////////////////////////////////////

    FlyToViewport(v3dOut.x(), v3dOut.y(), v3dOut.z());

    FireEventNodeAdd(this, spNode, getSceneData()->asGroup(), filename).fireEvent();

    m_aLayerFilePath->Add( CString(filename.c_str()) );

    return true;
}

bool CViewer3D::AddNode(osg::Node* pNode, osg::Node* pParentNode)
{
    if( pNode == NULL )
        return false;

    osg::ref_ptr<osg::Group> spGroup = NULL;
    if( pParentNode == NULL )
        spGroup = getSceneData()->asGroup();
    else
        spGroup = pParentNode->asGroup();

    if( spGroup == NULL )
        return false;

    bool bResetView = true;
    int nChildNum = spGroup->getNumChildren();

    //if( nChildNum <= 0 )
    //	bResetView = true;

    osg::ref_ptr<CThreadSafeUpdateCallback> spCallback = dynamic_cast<CThreadSafeUpdateCallback*>(spGroup->getUpdateCallback());
    if( spCallback != NULL )
        spCallback->AddChild(spGroup.get(), pNode);
    else
    {
        Lock();
        if( !(spGroup->addChild(pNode)) )
        {
            Unlock();
            return false;
        }
        Unlock();
    }

    FireEventNodeAdd(this, pNode, spGroup.get(), "").fireEvent();

    if( bResetView && m_spViewer != NULL )
    {
        //CenterNode(pNode, 0.0);
        // 		osg::ref_ptr<osgGA::GUIEventAdapter> spGUIEventAdapter = m_spViewer->getEventQueue()->createEvent();
        // 		if( spGUIEventAdapter!=NULL )
        // 			m_spViewer->getCameraManipulator()->home(*spGUIEventAdapter, *m_spViewer);
    }

    m_spStatesetManipulator->setLightingEnabled(true);
    m_spStatesetManipulator->setLightingEnabled(false);

    return true;
}

bool CViewer3D::RemoveNode(osg::Node* pNode, osg::Node* pParentNode)
{
    if( pNode == NULL )
        return false;

    osg::ref_ptr<osg::Group> spGroup = NULL;
    if( pParentNode == NULL )
    {
        if( pNode->getNumParents() > 0 )
            spGroup = pNode->getParent(0);
        else
            spGroup = getSceneData()->asGroup();
    }
    else
        spGroup = pParentNode->asGroup();

    if( spGroup == NULL )
        return false;

    bool bResetView = false;
    if( spGroup->getNumChildren() <= 0 )
        bResetView = true;

    osg::ref_ptr<CThreadSafeUpdateCallback> spCallback = dynamic_cast<CThreadSafeUpdateCallback*>(spGroup->getUpdateCallback());
    if( spCallback != NULL )
        spCallback->RemoveChild(spGroup.get(), pNode);
    else
    {
        Lock();
        if( !(spGroup->removeChild(pNode)) )
        {
            Unlock();
            return false;
        }
        Unlock();
    }

    FireEventNodeRemove(this, pNode, spGroup.get()).fireEvent();
    return true;
}

bool CViewer3D::CenterNode(osg::Node* pNode, double dMinDis)
{

    if (m_spKeyswitchManipulator == NULL || pNode == NULL )
        return false;

    osg::ref_ptr<osgGA::StandardManipulator> spCameraManipulator = dynamic_cast<osgGA::StandardManipulator*>(m_spKeyswitchManipulator->getCurrentMatrixManipulator());
    if( spCameraManipulator == NULL )
        return false;

	
	osg::Group* group = pNode->asGroup();
	if (group != nullptr)
	{
		if (group->getNumChildren()>1)
		{	
			if (group->getChild(0)->getName() == "object")
			{
				pNode = group->getChild(0);
			}		
		}
	}
	const osg::BoundingSphere& bs = pNode->getBound();
    
    if (fabs(bs.radius()) <= DBL_EPSILON) // invalid node
        return false;

    osg::Matrix mat = matrixListtoSingle( pNode->getWorldMatrices() );
    osg::Vec3d newCenter2 = bs.center() * mat;

    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    spCameraManipulator->getTransformation(eye, center, up);

	
	/*osg::Matrixd& m = m_spViewer->getCamera()->getInverseViewMatrix();
	osg::Vec3d position0 = m.getTrans();
	osg::Quat rotation = m.getRotate();
	osg::Vec3d position1 = position0 + osg::Vec3d(0,0,500);
	osg::Vec3d position2 = bs.center();
	position2.z() = fabs(bs.radius()) < dMinDis ? dMinDis : fabs(bs.radius());

	osg::ref_ptr<osg::AnimationPath> spPath = new osg::AnimationPath();
	spPath->insert(0,osg::AnimationPath::ControlPoint(position0,rotation));
	rotation.x() = 1.450302;
	rotation.y() = -1.429334;
	rotation.z() = -0.701939;
	rotation.w() = 0.712236;
	spPath->insert(2,osg::AnimationPath::ControlPoint(position1,rotation));
	rotation.x() = 0.395355;
	rotation.y() = 0.003596;
	rotation.z() = 0.008354;
	rotation.w() = 0.918483;
	spPath->insert(3.5,osg::AnimationPath::ControlPoint(position2,rotation));
	spPath->setLoopMode(osg::AnimationPath::NO_LOOPING);

	osg::ref_ptr<osgGA::AnimationPathManipulator> apm = new osgGA::AnimationPathManipulator(spPath.get());
	osg::ref_ptr<osgGA::CameraManipulator> _oldManipulator = m_spViewer->getCameraManipulator();
	m_spViewer->setCameraManipulator(apm.get());*/

	
	if (center == osg::Vec3d(0,0,0))
	{
		Lock();
		osg::Vec3d newCenter = bs.center();// * mat;
		osg::Vec3d newEye = eye - center;
		newEye.normalize();
		newEye *= fabs(bs.radius()) < dMinDis ? dMinDis : fabs(bs.radius());
		newEye += newCenter;
		spCameraManipulator->setTransformation(newEye, newCenter, up);
		Unlock();
		return false;
	}
	osg::Vec3d step = (bs.center()-center)/100;
	double sd = fabs(bs.radius()) < dMinDis ? dMinDis : fabs(bs.radius());
	sd/=100;
	for (int i = 1;i<=100;i++)
	{
		osg::Vec3d newCenter = center + osg::Vec3d(step.x()*i,step.y()*i,step.z()*i);// * mat;
		osg::Vec3d newEye = eye - center;
		newEye.normalize();
		newEye *=(sd*i);
		newEye += newCenter;
	
		spCameraManipulator->setTransformation(newEye, newCenter, up);
		Sleep(20);
	}
	return false;
}

bool CViewer3D::HighlightNode(osg::Node* pNode, bool bHighlight)
{
    if( bHighlight )
        FireEventNodeSelected(this, pNode).fireEvent();
    return false;
}

#define VP_MIN_DURATION      2.0     // minimum fly time.
#define VP_METERS_PER_SECOND 2500.0  // fly speed
#define VP_MAX_DURATION      2.0 //8.0     // maximum fly time.

bool CViewer3D::FlyToViewport(double x, double y, double z, double range /* = 2000 */)
{
#ifdef _2D

#else

    Viewpoint currentVP =  m_spEarthManipulator->getViewpoint();
    Viewpoint vp("XXX", x, y, z,  // longitude, latitude, altitude
                 currentVP.getHeading(), currentVP.getPitch(), range); // heading, pitch, range

    double distance = currentVP.focalPoint()->distanceTo(vp.focalPoint().get());
    double duration = osg::clampBetween(distance / VP_METERS_PER_SECOND, VP_MIN_DURATION, VP_MAX_DURATION);
    m_spEarthManipulator->setViewpoint( vp, duration );

#endif

    return true;
}

std::string CViewer3D::GetFileContent(std::string FilePath)
{
    std::ifstream _fp;
    std::string fileContent;
    _fp.open(FilePath, std::ios_base::in);
    if( _fp.is_open() )
    {
        std::string line;
        while( std::getline(_fp, line) )
        {
            fileContent += line;
        }
        _fp.close();
    }

    return fileContent;
}

std::wstring CViewer3D::RoundOffToString(double f, int n)
{
    std::wstringstream ss;
    int prec = std::numeric_limits<double>::digits10;
    ss.precision(prec);
    ss << f;

    std::wstring s = ss.str();
    int nDot = s.find(L".");
    return s.substr(0, nDot + 1 + n);
}

void CViewer3D::SetProjectFilePath(CString sPath)
{
	USES_CONVERSION;

	m_sProjectFilePath = sPath;
	m_aLayerFilePath->RemoveAll();
	m_aPathFilePath->RemoveAll();
	m_aLayerCheck->RemoveAll();

	FireEventAnything2(this, "PROJECTOPEN", W2A(sPath.GetBuffer(0))).fireEvent();
	FireEventProjectOpen(W2A(sPath.GetBuffer(0))).fireEvent();
}

void CViewer3D::SetDefLayer(CString sPath)
{
	USES_CONVERSION;

	m_sDefLayer = sPath;
	FireEventSetDefLayer(W2A(m_sDefLayer.GetBuffer(0))).fireEvent();
}

void CViewer3D::SetRecordPath(std::string sPath)
{
	USES_CONVERSION;
	m_sPath = sPath;
	FireEventAnything2(this, "PATHACTIVE", m_sPath).fireEvent();
	/*if (m_spPathHandler != nullptr)
	{
		m_spViewer->removeEventHandler(m_spPathHandler);
	}
	// add the record camera path handler
	m_spPathHandler = new osgViewer::RecordCameraPathHandler(sPath);
	m_spViewer->addEventHandler(m_spPathHandler);
	m_spPathHandler->setKeyEventToggleRecord(osgGA::GUIEventAdapter::KEY_A);
	*/
}

std::string CViewer3D::GetRecordPath()
{
	return m_sPath;
}

void CViewer3D::SetOSG_MAX_PAGEDLOD(int nMax)
{
	if (m_nOSG_MAX_PAGEDLOD != nMax)
	{
		m_nOSG_MAX_PAGEDLOD = nMax;
		std::string v = "OSG_MAX_PAGEDLOD=" + int2str(m_nOSG_MAX_PAGEDLOD);
		putenv(v.c_str());
		FireEventAnything2(this, "PROJECTCHANGED", "").fireEvent();
	}
}

void CViewer3D::SetOSG_NUM_DATABASE_THREADS(int nThreads)
{
	if (m_nOSG_NUM_DATABASE_THREADS != nThreads)
	{
		m_nOSG_NUM_DATABASE_THREADS = nThreads;
		std::string v = "OSG_NUM_DATABASE_THREADS=" + int2str(m_nOSG_NUM_DATABASE_THREADS);
		putenv(v.c_str());
		FireEventAnything2(this, "PROJECTCHANGED", "").fireEvent();
	}
}

void CViewer3D::SetScreenNum(int nNum)
{
	if (m_nScreenNum != nNum)
	{
		m_nScreenNum = nNum;
		FireEventAnything2(this, "PROJECTCHANGED", "").fireEvent();
	}
}

CString CViewer3D::GetViewportPos()
{
	 if (m_spKeyswitchManipulator == NULL)
        return false;

    osg::ref_ptr<osgGA::StandardManipulator> spCameraManipulator = dynamic_cast<osgGA::StandardManipulator*>(m_spKeyswitchManipulator->getCurrentMatrixManipulator());
    if( spCameraManipulator == NULL )
        return false;

    osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;
    spCameraManipulator->getTransformation(eye, center, up);

	CString s;
	s.Format(L"%f,%f,%f,%f,%f,%f,%f,%f,%f", eye.x(),eye.y(),eye.z(),center.x(),center.y(),center.z(),up.x(),up.y(),up.z());
	return s;
}

void CViewer3D::SetViewport(CString s)
{
	CStringArray a;
	AnalyzeData(a, s, L",");

	Lock();

	osg::Vec3d eye;
    osg::Vec3d center;
    osg::Vec3d up;

	eye.set(_tstof(a.GetAt(0)),_tstof(a.GetAt(1)),_tstof(a.GetAt(2))); 
	center.set(_tstof(a.GetAt(3)),_tstof(a.GetAt(4)),_tstof(a.GetAt(5))); 
	up.set(_tstof(a.GetAt(6)),_tstof(a.GetAt(7)),_tstof(a.GetAt(8))); 

    osg::ref_ptr<osgGA::StandardManipulator> spCameraManipulator = dynamic_cast<osgGA::StandardManipulator*>(m_spKeyswitchManipulator->getCurrentMatrixManipulator());
	spCameraManipulator->setTransformation(eye, center, up);

	Unlock();
}

END_NAMESPACE_EARTHMATRIX