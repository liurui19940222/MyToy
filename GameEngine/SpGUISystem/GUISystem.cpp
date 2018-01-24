#include "GUISystem.h"
#include "SpRendering\MeshFactory.h"

USING_NAMESPACE_GUI;

GUISystem::GUISystem()
{
}


GUISystem::~GUISystem()
{
}

void GUISystem::StartUp()
{
	m_SharedMesh = _MeshFactory->CreateBuffer(EMeshType::Quad);
}

void GUISystem::ShutDown()
{
	
}