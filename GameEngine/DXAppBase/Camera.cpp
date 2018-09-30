#include "Camera.h"

using namespace dxgame;
using namespace DirectX;

void Camera::lookAt(XMFLOAT3 eye, XMFLOAT3 center, XMFLOAT3 up)
{
	m_ViewMatrix = XMMatrixLookAtLH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&center), DirectX::XMLoadFloat3(&up));
}

void Camera::perspective(float aspect, float fov, float znear, float zfar)
{
	m_ProjMatrix = XMMatrixPerspectiveFovLH(fov, aspect, znear, zfar);
}

void Camera::ortho(float viewWidth, float viewHeight, float znear, float zfar)
{
	m_ProjMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, znear, zfar);
}