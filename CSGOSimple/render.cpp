#include "render.hpp"

#include <mutex>

#include "features/visuals.hpp"
#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "fonts/fonts.hpp"
#include "helpers/math.hpp"

ImFont* g_pDefaultFont;
ImFont* g_pSecondFont;


void Render::Initialize()
{
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(InputSys::Get().GetMainWindow());
	ImGui_ImplDX9_Init(g_D3DDevice9);

	GetFonts();
}

void Render::BeginScene()
{
	Render::Get().ClearDrawData();

	if (g_EngineClient->IsInGame() && g_LocalPlayer)
	{
		Visuals::Get().AddToDrawList();
	}

	// swap given data to safe container
	Render::Get().SwapDrawData();
}

void Render::GetFonts() {

	// menu font
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		Fonts::Droid_compressed_data,
		Fonts::Droid_compressed_size,
		14.f);
	
	// esp font
	g_pDefaultFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		Fonts::Droid_compressed_data,
		Fonts::Droid_compressed_size,
		18.f);
	

	// font for watermark; just example
	g_pSecondFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		Fonts::Cousine_compressed_data,
		Fonts::Cousine_compressed_size,
		18.f); 
}

