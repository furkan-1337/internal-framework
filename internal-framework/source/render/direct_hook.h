#pragma once
#include "../render/directx/directx.h"

namespace DirectHook
{
	typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	inline Present oPresent = nullptr;

	inline ID3D11Device* pDevice = nullptr;
	inline ID3D11DeviceContext* pContext = nullptr;
	inline ID3D11RenderTargetView* mainRenderTargetView = nullptr;

	inline WNDPROC oWndProc = NULL;

	bool Setup();
}