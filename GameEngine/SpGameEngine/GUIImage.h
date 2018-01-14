#ifndef _GUI_IMAGE_H_
#define _GUI_IMAGE_H_

#include"GUISystem.h"
#include"SpRendering\Texture.h"

namespace guisystem {

	class ENGINE_API CGUIImage : public CGUIWidget
	{
		REFLECT_CLASS(CGUIImage)
	private:
		Vector2 m_uvs[4];
		CTexture* m_texture;

	protected:
		virtual void Render(Matrix4x4& modelMatrix, Matrix4x4& viewMatrix, Matrix4x4& projectionMatrix) override;

	public:
		CGUIImage();
		CGUIImage* SetTexture(CTexture* texture);
	};

}
#endif