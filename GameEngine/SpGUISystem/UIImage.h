#pragma once

#include"UIView.h"
#include"SpRendering\Sprite.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(UIImage) class UIImage : public UIView
{
	GENERATE_UI_BODY()
public:
	UIImage();

	inline PSprite GetSprite() const { return m_Sprite; }

	void SetSprite(PSprite sprite, bool setToRawSize = false);
	void SetRawSize();

protected:
	virtual TexcoordRange GetTexcoordRange() override;
	virtual uint GetTextureId() override;
	virtual PTexture GetTexture() override;

private:
	PSprite			m_Sprite;
};

END_NAMESPACE_GUI
