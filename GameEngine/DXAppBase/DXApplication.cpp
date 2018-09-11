#include "DXApplication.h"
#include "Timer.h"
#include "string_ext.h"
#include "defs.h"
#include "..\SpCommon\Debug.h"

using namespace dxgame;
using namespace std;

DXApplication::DXApplication() : m_Running(false), m_Exiting(false)
{
	init();
}

DXApplication::~DXApplication()
{
}

void DXApplication::init() 
{
	// init window
	m_Window = make_shared<Window>(this);
	try 
	{
		m_Window->init();
	}
	catch (std::exception ex)
	{
		spgameengine::Debug::Log(ex.what());
		m_Exiting = true;
	}

	// init timer
	try 
	{
		m_Timer = TimerService::getTimer();
	}
	catch (std::exception ex)
	{
		spgameengine::Debug::Log(ex.what());
		m_Exiting = true;
	}

	// init d3d
	m_DXGraphics = make_shared<DXGraphics>();
	try 
	{
		m_DXGraphics->init(m_Window->getHwnd());
	}
	catch (std::exception ex)
	{
		m_Exiting = true;
	}

	spgameengine::Debug::Log("dxappliction init was successful.");
}

void DXApplication::run() 
{
	float frameBeginning = 0.0f;
	float dt = 1.0f / 30.0f;
	MSG msg = { 0 };
	while (!m_Exiting)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				m_Exiting = true;
				break;
			}
		}
		if (m_Exiting)
			break;
		//if (!m_Running)
		//	continue;
		m_Timer->beginFrame();
		update(m_Timer->getDeltaTime());
		render();
		m_Timer->endFrame();
	}
}

void DXApplication::update(float deltaTime)
{

}

void DXApplication::render()
{
	m_DXGraphics->clearBuffers();
	m_DXGraphics->present();
}

/*---------------Window Event Functions-----------------*/
void DXApplication::OnCreated() 
{
	spgameengine::Debug::Log("game window was created successfully.");
}

void DXApplication::OnSizeChanged(int width, int height) 
{
	spgameengine::Debug::Log("game window size changed width:%d height:%d", width, height);
	if (m_DXGraphics)
	{
		m_DXGraphics->resize(width, height);
	}
}

void DXApplication::OnActivate(bool active) 
{
	spgameengine::Debug::Log("game window activated isactive:%d", (int)active);
	m_Running = active;
	if (m_Timer)
	{
		if (m_Running) m_Timer->start();
		else m_Timer->pause();
	}
}

void DXApplication::OnDestroy() 
{
	spgameengine::Debug::Log("game window is destroying.");
	m_Exiting = true;
}