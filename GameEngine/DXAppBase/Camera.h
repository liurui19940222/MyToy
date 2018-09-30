#pragma once

#include<DirectXMath.h>

using namespace DirectX;

namespace dxgame {

	class Camera
	{
	public:
		Camera() = default;
		~Camera() = default;

		void lookAt(XMFLOAT3 eye, XMFLOAT3 center, XMFLOAT3 up);
		void perspective(float aspect, float fov, float znear, float zfar);
		void ortho(float viewWidth, float viewHeight, float znear, float zfar);

		inline XMMATRIX getViewMatrix() const { return m_ViewMatrix; }
		inline XMMATRIX getProjMatrix() const { return m_ProjMatrix; }

	private:
		XMMATRIX	m_ViewMatrix;
		XMMATRIX	m_ProjMatrix;
	};

}