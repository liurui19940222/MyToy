#include "Camera.h"


CCamera::CCamera(string name) : CGameObject(name){ }

CCamera::~CCamera(){ }

void CCamera::OnUpdate()
{
	
}

void CCamera::OnRender()
{
	gluLookAt(position.x, position.y, position.z, position.x + forward.x, position.y + forward.y, position.z + forward.z, up.x, up.y, up.z);
}
