#include "GraphicsEngine.h"
#include <exception>
#include <vector>

namespace Graphics
{
	GraphicsEngine::GraphicsEngine(bool CreateForDebug)
	{
		UINT flag;
		if (CreateForDebug)
			flag = DXGI_CREATE_FACTORY_DEBUG;

		//create the factory
		if (FAILED(CreateDXGIFactory2(flag, IID_PPV_ARGS(&Factory))))
			throw std::exception("Failed to create DXGI Factory");


		//now we initialize DX

		//specify the DX levels we will accept
		D3D_FEATURE_LEVEL featureLevels[] = 
		{
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0
		};

		//create the device
		UINT deviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_SINGLETHREADED;
		if (CreateForDebug)
			deviceFlags |= D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG
			| D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUGGABLE; //forces software render

		ComPtr<ID3D11Device> dev;
		if (FAILED(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			deviceFlags,
			featureLevels,
			_ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			dev.GetAddressOf(),
			&FeatureLevel,
			nullptr)))
		{
			//no dice. lets try for a warp device
			if (FAILED(D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_WARP,
				nullptr,
				deviceFlags,
				featureLevels,
				_ARRAYSIZE(featureLevels),
				D3D11_SDK_VERSION,
				dev.GetAddressOf(),
				&FeatureLevel,
				nullptr)))
			{
				throw std::exception("Failed to create device");
			}
			else
				RenderingInSoftware = true;
		}

		//we have a device, but lets make it a device4
		if (FAILED(dev.As(&Device)))
			throw std::exception("Failed to create device");

		Device->GetImmediateContext3(DeviceContext.GetAddressOf());

		if (!GetRefreshRate())
			throw std::exception("Failed to poll refresh rate");
	}

	bool GraphicsEngine::GetRefreshRate()
	{
		ComPtr<IDXGIAdapter1> adapt;
		//get the primary adapter
		if (FAILED(Factory->EnumAdapters1(0, adapt.GetAddressOf())))
			return false;
		//try to convert it to a adapter3
		if (FAILED(adapt.As(&Adapter)))
			return false;

		//get an output
		ComPtr<IDXGIOutput> adapterOutput;
		if (FAILED(Adapter->EnumOutputs(0, adapterOutput.GetAddressOf())))
			return false;

		//get the number of output modes
		UINT numModes;
		if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr)))
			return false;

		std::vector<DXGI_MODE_DESC> displayModeList;
		displayModeList.resize(numModes);

		//for real this time
		if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, &displayModeList[0])))
			return false;

		//and then just take the first one because I'm lazy
		RefreshRate = displayModeList[0].RefreshRate;


		//clear the comptr we don't need
		adapterOutput.Reset();
		return true;
	}
}
