#ifndef _GUI_LABLE_H_
#define _GUI_LABLE_H_

#include"GUISystem.h"
#include"Texture.h"
#include"FontRenderer.h"

namespace guisystem {

	class ENGINE_API CGUILabel : public CGUIWidget, public CFontRenderer
	{
		REFLECT_CLASS(CGUILabel)
	protected:
		virtual void OnStart() override;
		virtual CFontRenderer* SetTextRect(SRect2D rect) override;
	public:
		CGUILabel();

		virtual float GetPixelScale() override;
		virtual CGUIWidget* SetRect(SRect2D rect) override;
		virtual CGUIWidget* SetWidth(float width) override;
		virtual CGUIWidget* SetHeight(float height) override;

		virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix) override;
		virtual void RenderDebug(Matrix4x4& modelMatrix) override;
	};

}

#endif