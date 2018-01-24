#ifndef _LIGHT_H_
#define _LIGHT_H_

#include"SpCommon\EngineDefine.h"
#include"SpCommon\Math.h"
#include"RenderTexture.h"
#include"Shader.h"
#include<vector>

using namespace std;

#define LIGHT_MAX_NUM 12

namespace spgameengine
{

	enum class ELightType
	{
		Directional = 0,
		Point = 1,
		Spot = 2,
	};

	SMART_CLASS(Light) class Light
	{
	protected:
		static Color color_buffer[LIGHT_MAX_NUM];
		static Vector3 pos_buffer[LIGHT_MAX_NUM];
		static float intensity_buffer[LIGHT_MAX_NUM];
		static int type_buffer[LIGHT_MAX_NUM];
		static Matrix4x4 shadowMapMatrix;
		static PRenderTexture shadowMap;
		static vector<PLight> lights;
		static int shadowCasterIndex;

	protected:
		Color m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float m_intensity = 1.0f;
		ELightType m_type = ELightType::Directional;
		bool m_castShadow = false;

	public:

		static void SetShadowMapSize(int width, int height);
		static PRenderTexture GetShadowMap();
		static Matrix4x4 GetShadowMapMatrix();
		static void SetUniformParams(PShader shader);
		static PLight PrepareLight();
		inline virtual void SetColor(Color color) { m_color = color; }
		inline virtual void SetIntensity(float intensity) { m_intensity = intensity; }
		inline virtual void SetType(ELightType type) { m_type = type; }
		inline virtual void SetCastShadow(bool isCastShadow) { m_castShadow = isCastShadow; }

		inline Color GetColor() const { return m_color; }
		inline float GetIntensity() const { return m_intensity; }
		inline ELightType GetType() const { return m_type; }
		inline bool IsCastShadow() const { return m_castShadow; }

		virtual Vector3 GetLightPosOrDirection() const = 0;
	};

}

#endif