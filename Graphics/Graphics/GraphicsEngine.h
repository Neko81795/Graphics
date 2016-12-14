#pragma once

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <dxgi1_5.h>
#include <d3dcommon.h>
#include <d3d11_4.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>

using Microsoft::WRL::ComPtr;

namespace Graphics
{
	class GraphicsEngine
	{
	public:
		ComPtr<ID3D11Device4> Device;
		ComPtr<ID3D11DeviceContext3> DeviceContext;
		ComPtr<ID3D11RasterizerState2> RasterState;
		ComPtr<IDXGIFactory4> Factory;
		ComPtr<IDXGIAdapter3> Adapter;
		DXGI_RATIONAL RefreshRate;
		bool RenderingInSoftware;

		GraphicsEngine(bool CreateForDebug = false);

	private:
		D3D_FEATURE_LEVEL FeatureLevel;
		bool GetRefreshRate();
	};
}
