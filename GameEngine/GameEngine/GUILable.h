#ifndef _GUI_LABLE_H_
#define _GUI_LABLE_H_

#include"GUISystem.h"
#include"Texture.h"
#include"FontRenderer.h"

namespace guisystem {

	class CGUILable : public CGUIWidget, public CFontRenderer
	{
		REFLECT_CLASS(CGUILable)
	protected:
		virtual void OnStart() override;
		virtual CFontRenderer* SetTextRect(SRect2D rect) override;
	public:
		CGUILable();

		virtual float GetPixelScale() override;
		virtual CGUIWidget* SetRect(SRect2D rect) override;
		virtual CGUIWidget* SetWidth(float width) override;
		virtual CGUIWidget* SetHeight(float height) override;
	};

}

#endif