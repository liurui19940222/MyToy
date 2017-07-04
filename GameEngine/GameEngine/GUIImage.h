#ifndef _GUI_IMAGE_H_
#define _GUI_IMAGE_H_

#include"GUISystem.h"
#include"Texture.h"

using namespace guisystem;

class CGUIImage : public CGUIWidget
{
	REFLECT_CLASS(CGUIImage)
private:
	Vector2 m_uvs[4];
	CTexture* m_texture;

protected:
	virtual void OnUIRender() override;

public:
	CGUIImage();
	CGUIImage* SetTexture(CTexture* texture);
};

#endif