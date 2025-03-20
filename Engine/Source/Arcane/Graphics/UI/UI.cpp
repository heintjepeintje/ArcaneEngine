#include "UI.hpp"

#include <imgui.h>

namespace Arcane::UI {

	void Init() {
		ImGuiIO &io = ImGui::GetIO();

		io.BackendPlatformName = "Win32";
		io.BackendRendererName = "Arcane";
	}

	void BeginUI() {
		ImGui::NewFrame();
	}

	void EndUI() {
		ImGui::EndFrame();
	}

	void DrawUI() {
		
	}


}