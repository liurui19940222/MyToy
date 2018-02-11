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

class CharacterPrimitiveBase : public Object
{
public:
	int						m_Left;
	int						m_Top;
	int						m_AdvanceX;
	float					m_Width;
	float					m_Height;
	Vector3					m_Position;

	virtual ~CharacterPrimitiveBase();
	CharacterPrimitiveBase(int left_padding, int top, int advance_x, int width, int height, float pixelScale);
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color) = 0;
};

SMART_CLASS(CharacterPrimitiveSmart) class CharacterPrimitiveSmart : public CharacterPrimitiveBase
{
public:
	PMeshBufferTexcoord		m_Buffer;
	PMaterial				m_Material;
	PSprite					m_Sprite;

	virtual ~CharacterPrimitiveSmart();
	CharacterPrimitiveSmart(int left_padding, int top, int advance_x, int width, int height, float pixelScale, PSprite sprite, PMaterial mat);
	virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 pos, Vector3 size, Color color) override;
};

SMART_CLASS(TextOneLineData) class TextOneLineData : public Object
{
public:
	float								m_LineWidth;
	float								m_LineHeight;
	vector<PCharacterPrimitiveSmart>	primitives;
	TextOneLineData();
	~TextOneLineData();
};

class FontRenderer
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
	wstring					m_Text;
	PTrueTypeFont			m_Font;
	SRect2D					m_Rect;
	EAlignment				m_Alignment;
	EAlignmentHorizontal	m_AlignmentH;
	EAlignmentVertical		m_AlignmentV;
	vector<PTextOneLineData>		 m_LineDatas;
	vector<PCharacterPrimitiveSmart> m_Primitives;

protected:
	void ClearPrimitive();
	void ClearLineData();
	virtual float GetPixelScale();
	float GetOffsetX(int line_index);
	float GetOffsetY();
	void Rebuild();
	void Init(PTrueTypeFont font, int font_size, float interval_x, Color color, EAlignment alignment, SRect2D rect);
	void RenderAllPrimitives(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix, Vector3 offset, const Color& color);

public:
	void OnRender(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix);
	void OnRenderDebug(Matrix4x4& modelMatrix);
	virtual FontRenderer* SetTextRect(SRect2D rect);
	SRect2D GetTextRect();
	FontRenderer* SetFont(PTrueTypeFont font);
	PTrueTypeFont GetFont();
	FontRenderer* SetText(const wstring text);
	const wstring& GetText();
	FontRenderer* SetIntervalX(float x);
	FontRenderer* SetIntervalY(float y);
	FontRenderer* SetFontSize(int size);
	FontRenderer* SetColor(Color color);
	FontRenderer* SetSingleLine(bool isSingle);
	FontRenderer* SetTextAlignment(EAlignment alignment);
	FontRenderer* SetEffect(EFontEffect effect);
	FontRenderer* SetEffectVector(const Vector3& v);
	FontRenderer* SetEffectColor(const Color& color);
	PTextOneLineData GetLineData(int rowIndex);
	static PMaterial GetDefaultMaterial();
};

END_NAMESPACE_ENGINE
#endif