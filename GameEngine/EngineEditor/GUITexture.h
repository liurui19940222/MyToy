#ifndef _GUI_TEXTURE_H_
#define _GUI_TEXTURE_H_

#include"GUIElement.h"

class CGUITexture : public CGUIElement
{
private:
	CTexture* m_texture;

public:
	CGUITexture();

	virtual void OnRender() override;
	virtual void OnStateChanged() override;

	CGUITexture* SetTexture(CTexture* texture);
	CGUITexture* SetRawSize();
};

#endif