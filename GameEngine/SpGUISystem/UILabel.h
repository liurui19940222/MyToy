#pragma once

#include"GUIDefines.h"
#include"UIView.h"

BEGIN_NAMESPACE_GUI

SMART_CLASS(UILabel) class UILabel : public UIView
{
	GENERATE_UI_BODY()
public:
	UILabel();

};

END_NAMESPACE_GUI