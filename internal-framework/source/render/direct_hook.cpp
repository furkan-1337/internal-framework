#include "direct_hook.h"
#include "font/font.h"
#include <source/thirdparty/imgui/imgui.h>
#include <source/thirdparty/imgui/backends/imgui_impl_win32.h>
#include <source/thirdparty/imgui/backends/imgui_impl_dx11.h>
#include <source/memory/hook/hook_engine.h>
#include <source/debug/Debug.h>
#include <source/menu/menu.h>
#include <source/cheat/cheat.h>

extern HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool DirectHook::Setup()
{
	auto methods = DirectX::GetMethodTable(D3DVersion::Direct3D11);
	if (!HookEngine::Hook((void*)methods[8], hkPresent, (void**)&DirectHook::oPresent))
	{
		Debug::Print("Present (0x%p) hook failed!", methods[8]);
		return false;
	}
	return true;
}

static bool IsImGuiInitialized = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!IsImGuiInitialized)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&DirectHook::pDevice)))
		{
			DirectHook::pDevice->GetImmediateContext(&DirectHook::pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			auto window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			DirectHook::pDevice->CreateRenderTargetView(pBackBuffer, NULL, &DirectHook::mainRenderTargetView);
			pBackBuffer->Release();
			DirectHook::oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			io.IniFilename = NULL;
			io.LogFilename = NULL;

			ImFontConfig icons_config;
			icons_config.FontDataOwnedByAtlas = false;

			io.Fonts->AddFontFromMemoryTTF((void*)fontRaw, sizeof(fontRaw), 16.5f, &icons_config);

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(DirectHook::pDevice, DirectHook::pContext);

			IsImGuiInitialized = true;
		}

		else
			return DirectHook::oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Menu::Render();
	Cheat::Render();

	ImGui::Render();

	DirectHook::pContext->OMSetRenderTargets(1, &DirectHook::mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return DirectHook::oPresent(pSwapChain, SyncInterval, Flags);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(DirectHook::oWndProc, hWnd, uMsg, wParam, lParam);
}