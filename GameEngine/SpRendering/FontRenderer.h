#ifndef _FONT_RENDERER_H_
#define _FONT_RENDERER_H_

#include"SpCommon\Math.h"
#include"FontManager.h"
#include"SpCommon\EngineDefine.h"
#include"SpRendering\Renderer.h"
#include"SpRendering\MeshBufferTexcoord.h"
#include"SpRendering\Texture2D.h"
#include"SpRendering\Material.h"
#include"SpRendering\Sprite.h"

BEGIN_NAMESPACE_ENGINE

#define TEXT_BUFFER_SIZE 2048

enum class EFontEffect {
	None,
	Shadow,
	Outline,
};

class CCharacterPrimitiveBase
{
public:
	int						m_Left;
	int						m_Top;
	int						m_AdvanceX;
	float					m_Width;
	float					m_Height;
	Vector3					m_Position;

	CCharacterPrimitiveBase(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels);
	virtual ~CCharacterPrimitiveBase();
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color) = 0;
};

class CCharacterPrimitiveSmart : public CCharacterPrimitiveBase
{
public:
	PMeshBufferTexcoord		m_Buffer;
	shared_ptr<Material>	m_Material;
	PSprite					m_Sprite;

	CCharacterPrimitiveSmart(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels, PSprite sprite);
	virtual ~CCharacterPrimitiveSmart();
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color) override;
};

class CCharacterPrimitiveFixed : public CCharacterPrimitiveBase
{
	PTexture2D				m_Texture;
public:
	CCharacterPrimitiveFixed(int left_padding, int top, int advance_x, int width, int height, float pixelScale, uint32* pixels);
	virtual ~CCharacterPrimitiveFixed();
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color) override;
};

class CTextOneLineData
{
public:
	float					m_LineWidth;
	float					m_LineHeight;

	CTextOneLineData();
	virtual ~CTextOneLineData();
	vector<CCharacterPrimitiveBase*> primitives;
};

class CFontRenderer
{
private:
	float					m_Interval_x;
	float					m_Interval_y;
	float					m_TotalHeight;
	int						m_FontSize;
	bool					m_bSingleLine;
	Vector3					m_EffectVector;
	Color					m_EffectColor;
	Color					m_Color;
	wchar_t					m_TextBuffer[TEXT_BUFFER_SIZE];
	EFontEffect				m_Effect = EFontEffect::None;
	ERenderType				m_RenderType = ERenderType::Smart;
	wstring					m_Text;
	CTrueTypeFont*			m_Font;
	SRect2D					m_Rect;
	EAlignment				m_Alignment;
	EAlignmentHorizontal	m_AlignmentH;
	EAlignmentVertical		m_AlignmentV;
	vector<CTextOneLineData*>		 m_LineDatas;
	vector<CCharacterPrimitiveBase*> m_Primitives;

protected:
	void ClearPrimitive();
	void ClearLineData();
	virtual float GetPixelScale();
	float GetOffsetX(int line_index);
	float GetOffsetY();
	void Rebuild();
	void Init(CTrueTypeFont* font, int font_size, float interval_x, Color color, EAlignment alignment, SRect2D rect);
	void RenderAllPrimitives(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 offset, const Color& color);

public:
	void OnRender(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix);
	void OnRenderDebug(Matrix4x4& modelMatrix);
	virtual CFontRenderer* SetTextRect(SRect2D rect);
	SRect2D GetTextRect();
	CFontRenderer* SetFont(CTrueTypeFont* font);
	CTrueTypeFont* GetFont();
	CFontRenderer* SetText(const wstring text);
	const wstring& GetText();
	CFontRenderer* SetIntervalX(float x);
	CFontRenderer* SetIntervalY(float y);
	CFontRenderer* SetFontSize(int size);
	CFontRenderer* SetColor(Color color);
	CFontRenderer* SetSingleLine(bool isSingle);
	CFontRenderer* SetTextAlignment(EAlignment alignment);
	CFontRenderer* SetRenderType(ERenderType type);
	CFontRenderer* SetEffect(EFontEffect effect);
	CFontRenderer* SetEffectVector(const Vector3& v);
	CFontRenderer* SetEffectColor(const Color& color);
	CTextOneLineData* GetLineData(int rowIndex);
};

END_NAMESPACE_ENGINE
#endif