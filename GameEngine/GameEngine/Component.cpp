#include "Component.h"


CComponent::CComponent() : Object()
{
}


CComponent::~CComponent()
{
}

void CComponent::OnStart(){}

void CComponent::OnUpdate(){}

void CComponent::OnRender(){}

void CComponent::OnDestroy(){}

CGameObject* CComponent::GetGameObject()
{
	return gameObject;
}