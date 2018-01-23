#pragma once

#include"SpCommon\EngineDefine.h"

class UIWidget
{
public:
	UIWidget();
	~UIWidget();

	UIWidget* SetWidth(float width);
	UIWidget* SetHeight(float height);

private:
	SRect2D					m_Rect;
};

