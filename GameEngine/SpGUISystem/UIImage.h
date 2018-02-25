#pragma once

#include"UIView.h"
#include"SpRendering\Sprite.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(UIImage) class UIImage : public UIView
{
	GENERATE_UI_BODY()
public:
	enum Type {
		Simple,
		Slice,
	};

	struct SliceData {
		vector<TexcoordRange>	m_TexcoordRanges;
		vector<Color>			m_Colors;
		vector<SRect2D>			m_RectList;
		vector<Matrix4x4>		m_ModelMatrices;
	};

	UIImage();

	inline PSprite GetSprite() const { return m_Sprite; }

	void SetSprite(PSprite sprite, bool setToRawSize = false);
	void SetRawSize();
	void SetImageType(Type type);

protected:
	virtual void CalcModelMatrix(Matrix4x4& baseMatrix) override;
	virtual const Matrix4x4& GetGlobalModelMatrix() const;
	virtual TexcoordRange GetTexcoordRange() override;
	virtual uint GetTextureId() override;
	virtual PTexture GetTexture() override;
	virtual void MakeData(vector<TexcoordRange>& texcoordRanges, vector<Color>& colors,
		vector<SRect2D>& rectList, vector<Matrix4x4>& modelMatrices) override;

private:
	void BuildSliceData();

	PSprite					m_Sprite;
	Type					m_Type;
	shared_ptr<SliceData>	m_SliceData;
};

END_NAMESPACE_GUI
