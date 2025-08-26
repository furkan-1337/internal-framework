#include "menu.h"
#include "theme/theme.h"
#include <Windows.h>

static bool exampleCheck = false;
static int exampleSlider = 5;
void Menu::Render()
{
	if (!ThemeApplied)
	{
		Theme::Apply();
		ThemeApplied = true;
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
		Show = !Show;

	if (!Show)
		return;

	ImGui::SetNextWindowSize({ 600, 400 }, ImGuiCond_Once);
	ImGui::Begin("[internal-framework]", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
	if (ImGui::CollapsingHeader("Header"))
	{
		ImGui::Text("Content");
		ImGui::Button("Button");
		ImGui::Checkbox("Checkbox", &exampleCheck);
		ImGui::SliderInt("Slider", &exampleSlider, 0, 10);
	}
	ImGui::End();
}