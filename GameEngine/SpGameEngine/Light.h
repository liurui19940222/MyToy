#ifndef _LIGHT_H_
#define _LIGHT_H_

#include"..\SpCommon\EngineDefine.h"
#include"Math.h"
#include"Component.h"
#include"RenderTexture.h"
#include"Shader.h"
#include<vector>

using namespace std;

#define LIGHT_MAX_NUM 12

enum class ELightType
{
	Directional = 0,
	Point = 1,
	Spot = 2,
};

class ENGINE_API CLight : public CComponent
{
private:
	static Color color_buffer[LIGHT_MAX_NUM];
	static Vector3 pos_buffer[LIGHT_MAX_NUM];
	static float intensity_buffer[LIGHT_MAX_NUM];
	static int type_buffer[LIGHT_MAX_NUM];
	static Matrix4x4 shadowMapMatrix;
	static CRenderTexture* shadowMap;
	static vector<CLight*> lights;
	static int shadowCasterIndex;

protected:
	Color m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
	float m_intensity = 1.0f;
	ELightType m_type = ELightType::Directional;
	bool m_castShadow = false;

public:

	static void SetShadowMapSize(int width, int height);
	static CRenderTexture* GetShadowMap();
	static CLight* PrepareLight();
	static void RenderShadowMap(CLight* light);
	static Matrix4x4 GetShadowMapMatrix();
	static void SetUniformParams(CShader* shader);

	inline virtual void SetColor(Color color) { m_color = color; }
	inline virtual void SetIntensity(float intensity) { m_intensity = intensity; }
	inline virtual void SetType(ELightType type) { m_type = type; }
	inline virtual void SetCastShadow(bool isCastShadow) { m_castShadow = isCastShadow; }

	inline Color GetColor() const { return m_color; }
	inline float GetIntensity() const { return m_intensity; }
	inline ELightType GetType() const { return m_type; }
	inline bool IsCastShadow() const { return m_castShadow; }
	Vector3 GetLightPosOrDirection() const;

	virtual void OnStart() override;
	virtual void OnDestroy() override;
};

#endif