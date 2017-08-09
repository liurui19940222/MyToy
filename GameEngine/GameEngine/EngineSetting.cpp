#include"EngineSetting.h"
#include"Config.h"

bool CEngineSetting::DrawGrid;
bool CEngineSetting::DrawGizmos;

void CEngineSetting::Init()
{
	DrawGrid = _Config->GetValue<bool>("debug", "draw_grid");
	DrawGizmos = _Config->GetValue<bool>("debug", "draw_gizmos");
}