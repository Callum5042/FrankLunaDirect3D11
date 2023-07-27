//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
// 
// Exercise 5:
//		Each output has a list of supported display modes (DXGI_MODE_DESC) for a given
//		pixel format. For each output (IDXGIOutput), show the widht, height and refresh
//		rate of each display mode the output supports for the DXGI_FORMAT_R8G8B8A8_UNORM
//		format using the IDXGIOutput::GetDisplayModeList method.
//		Example output for Exercise 2, 3, 4, and 5 is listed below. It is useful to use the
//		OutputDebugString function for quick output to the VC++ output window.
//
//***************************************************************************************

#include "d3dApp.h"
#include <sstream>
#include <vector>
 
class InitDirect3DApp : public D3DApp
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	InitDirect3DApp theApp(hInstance);
	
	if( !theApp.Init() )
		return 0;
	
	return theApp.Run();
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance)
: D3DApp(hInstance) 
{
}

InitDirect3DApp::~InitDirect3DApp()
{
}

bool InitDirect3DApp::Init()
{
	if(!D3DApp::Init())
		return false;

	// Get IDXGIFactory
	IDXGIDevice* dxgiDevice = 0;
	HR(md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	// Output hardware information
	int adapter_index = 0;
	IDXGIAdapter* adapter = nullptr;
	while (dxgiFactory->EnumAdapters(adapter_index, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapter_index++;

		// Adapter details
		DXGI_ADAPTER_DESC adapter_desc = {};
		adapter->GetDesc(&adapter_desc);

		// IDXGIAdapter::CheckInterfaceSupport cannot be used to determine if Direct3D 11 is supported (Only Direct3D 10.x)

		std::wstringstream ss;
		ss << "=====================================" << '\n';
		ss << "GPU: " << adapter_desc.Description << '\n';
		ss << "=====================================" << '\n';

		OutputDebugString(ss.str().c_str());

		// Get 
		int output_index = 0;
		IDXGIOutput* output = nullptr;
		while (adapter->EnumOutputs(output_index, &output) != DXGI_ERROR_NOT_FOUND)
		{
			output_index++;

			DXGI_OUTPUT_DESC output_desc = {};
			output->GetDesc(&output_desc);

			UINT num = 0;
			output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num, nullptr);

			std::vector<DXGI_MODE_DESC> display_mode(num);
			output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num, display_mode.data());

			std::wstringstream outputss;
			outputss << "=== Diplay: " << output_desc.DeviceName << '\n';

			for (auto& format : display_mode)
			{
				outputss << "- WIDTH = " << format.Width << " HEIGHT = " << format.Height << " REFRESH = " << format.RefreshRate.Numerator << "/" << format.RefreshRate.Denominator << '\n';
			}

			OutputDebugString(outputss.str().c_str());
		}
	}


	// Free up memory
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	return true;
}

void InitDirect3DApp::OnResize()
{
	D3DApp::OnResize();
}

void InitDirect3DApp::UpdateScene(float dt)
{

}

void InitDirect3DApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Blue));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	HR(mSwapChain->Present(0, 0));
}
