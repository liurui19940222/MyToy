#ifndef _GUI_CHECK_BOX_H_
#define _GUI_CHECK_BOX_H_

#include"GUIElement.h"
#include<SpGameEngine\Texture2D.h>

class CGUICheckBox : public CGUIElement
{
	typedef function<void(bool)> OnValueChanged;
private:
	CTexture* m_icon = NULL;
	bool m_selected;
	OnValueChanged m_callback;

public:
	CGUICheckBox();
	void SetIcon(CTexture* tex);
	void SetOnValueChangedListener(OnValueChanged callback);
	void SetSelected(bool isSelected);
	bool IsSelected();

	virtual void OnRender() override;
};

#endif