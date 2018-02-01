#pragma once

#define BEGIN_NAMESPACE_GUI namespace spgameengine {
#define END_NAMESPACE_GUI }
#define USING_NAMESPACE_GUI using namespace spgameengine;

#define GENERATE_UI_BODY() \
friend class UISystem; 