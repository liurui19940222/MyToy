#ifndef _FONT_RENDERER_H_
#define _FONT_RENDERER_H_

#include"..\SpCommon\Math.h"
#include"..\SpCommon\EngineDefine.h"
#include"FontManager.h"
#include"Renderer.h"
#include"MeshBufferTexcoord.h"
#include"Texture2D.h"
#include"Material.h"
#include"Sprite.h"
#include"FontMeshGenerator.h"
#include"Rendering.h"
#include"MeshBufferUIInstance.h"

BEGIN_NAMESPACE_ENGINE

#define TEXT_BUFFER_SIZE 2048

enum class EFontEffect {
	None,
	Shadow,
	Outline,
};

class FontRenderer : public FontMeshGenerator
{
private:
	Vector3					m_EffectVector;
	Color					m_EffectColor;
	Color					m_Color;
	EFontEffect				m_Effect = EFontEffect::None;
	IRenderingInterface*	m_RI;

public:
	FontRenderer(IRenderingInterface* ri);
	void OnRender(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix);
	void OnRenderDebug(Matrix4x4& modelMatrix);
	
	inline void SetColor(Color color) { m_Color = color; }
	inline void SetEffect(EFontEffect effect) { m_Effect = effect; }
	inline void SetEffectVector(const Vector3& v) { m_EffectVector = v; }
	inline void SetEffectColor(const Color& color) { m_EffectColor = color; }
	static PMeshBufferUIInstance GetDefaultBuffer();
};

END_NAMESPACE_ENGINE
#endif