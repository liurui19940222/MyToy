#ifndef _GUI_TEXTURE_H_
#define _GUI_TEXTURE_H_

#include"GUIElement.h"

class CGUITexture : public CGUIElement
{
private:
	CTexture* m_texture;
	bool m_managedTex = false;
public:
	CGUITexture();

	virtual void OnDestroy() override;
	virtual void OnRender() override;
	virtual void OnStateChanged() override;

	CGUITexture* SetTexture(CTexture* texture, bool managed = false);
	CGUITexture* SetRawSize();
	CGUITexture* ConstrainSize(float maxWidth, float maxHeight);
};

#endif