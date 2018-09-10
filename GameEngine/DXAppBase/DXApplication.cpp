#include "DXApplication.h"
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
}

void DXApplication::run() 
{
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
	}
}

void DXApplication::update(float deltaTime)
{

}

void DXApplication::render()
{
	
}

/*---------------Window Event Functions-----------------*/
void DXApplication::OnCreated() 
{
	spgameengine::Debug::Log("game window was created successfully.");
}

void DXApplication::OnSizeChanged(int width, int height) 
{
	spgameengine::Debug::Log("game window size changed width:%d height:%d", width, height);
}

void DXApplication::OnActivate(bool active) 
{
	spgameengine::Debug::Log("game window activated isactive:%d", (int)active);
	m_Running = active;
}

void DXApplication::OnDestroy() 
{
	spgameengine::Debug::Log("game window is destroying.");
	m_Exiting = true;
}