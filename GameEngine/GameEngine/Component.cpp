#include "Component.h"

IMPL_CLASS(CComponent)

CComponent::CComponent() : Object()
{
}


CComponent::~CComponent()
{
}

void CComponent::OnStart(){}

void CComponent::OnUpdate(){}

void CComponent::OnDestroy(){}

CGameObject* CComponent::GetGameObject()
{
	return gameObject;
}