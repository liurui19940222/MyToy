#include"SpImGui.h"
#include"..\imgui\imgui.h" 
#include"..\imgui\examples\imgui_impl_opengl3.h"
#include"..\SpCommon\Input.h"

USING_NAMESPACE_ENGINE;

bool spgameengine::ImGui_Init()
{
	ImGui::CreateContext();
	if (!ImGui_ImplOpenGL3_Init("#version 410"))
		return false;
	ImGui::StyleColorsDark();
	return true;
}

void spgameengine::ImGui_UpdateInput()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MousePos.x = Input::InputMousePosition(true).x;
	io.MousePos.y = Input::InputMousePosition(true).y;
	io.MouseWheel = Input::GetAxis("Scroll");
	if (Input::GetMouseDown(EMouseKey::Left))
	{
		io.MouseDown[0] = true;
	}
	if (Input::GetMouseDown(EMouseKey::Right))
	{
		io.MouseDown[1] = true;
	}
	if (Input::GetMouseDown(EMouseKey::Center))
	{
		io.MouseDown[2] = true;
	}
	if (Input::GetMouseUp(EMouseKey::Left))
	{
		io.MouseDown[0] = false;
	}
	if (Input::GetMouseUp(EMouseKey::Right))
	{
		io.MouseDown[1] = false;
	}
	if (Input::GetMouseUp(EMouseKey::Center))
	{
		io.MouseDown[2] = false;
	}
}

void spgameengine::ImGui_BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
}

void spgameengine::ImGui_EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}