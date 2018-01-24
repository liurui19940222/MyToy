#pragma once

#include"SpRendering\Material.h"
#include"SpRendering\MeshBuffer.h"
#include"GUIDefines.h"

BEGIN_NAMESPACE_GUI

class GUISystem
{
public:
	GUISystem();
	~GUISystem();

	void StartUp();
	void ShutDown();

private:
	PMeshBuffer			m_SharedMesh;
	PMaterial			m_SharedMaterial;
};

END_NAMESPACE_GUI