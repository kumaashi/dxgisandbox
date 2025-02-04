#include <stdio.h>
#include <wchar.h>
#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib, "dxgi.lib")

int main() {
	//enumeration outputs
	auto enumOutputs = [&](auto dxgifactory) {
		HRESULT hret = S_OK;
		for (int i = 0; hret == S_OK; i++) {
			IDXGIOutput *dxgioutput = nullptr;
			hret = dxgifactory->EnumOutputs(i, &dxgioutput);
			if (dxgioutput) {
				DXGI_OUTPUT_DESC desc;
				dxgioutput->GetDesc(&desc);
				wprintf(L"\t\tDeviceName=%ls\n", desc.DeviceName);
				dxgioutput->Release();
			}
		}
	};

	//enumeration to the card.
	auto enumAdapters = [&](auto dxgifactory) {
		HRESULT hret = S_OK;
		for (int i = 0; hret == S_OK; i++) {
			IDXGIAdapter *dxgiadapter = nullptr;
			hret = dxgifactory->EnumAdapters(i, &dxgiadapter);
			if (hret == S_OK) {
				DXGI_ADAPTER_DESC desc;
				dxgiadapter->GetDesc(&desc);
				wprintf(L"\tDescription=%ls\n", desc.Description);
				enumOutputs(dxgiadapter);
				dxgiadapter->Release();
			}
		}
	};

	//createFactory
	auto createFactory = [&]() {
		IDXGIFactory6 *dxgifactory = nullptr;
		CreateDXGIFactory1(IID_PPV_ARGS(&dxgifactory));
		if (dxgifactory) {
			printf("dxgifactory=%p\n", dxgifactory);
			enumAdapters(dxgifactory);
			BOOL allowTearing = FALSE;
			dxgifactory->CheckFeatureSupport(
				DXGI_FEATURE_PRESENT_ALLOW_TEARING,
				&allowTearing,
				sizeof(allowTearing));
			printf("DXGI_FEATURE_PRESENT_ALLOW_TEARING=%d\n", allowTearing);
			dxgifactory->Release();
		}
	};

	createFactory();

	return 0;
}

