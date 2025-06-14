//=================================================
//
// 4. Direct3D 12 Initialization
//	
// - Direct3D 초기화
// - 매 프레임 기본적으로 해야 할 일
// 
//=================================================


#include "../common/d3dApp.h"
#include "DirectXColors.h"

class InitDirect3DApp : public D3DApp {

public:
	InitDirect3DApp(HINSTANCE hInstnace);
	~InitDirect3DApp();

	virtual bool Initialize() override;

private:

	virtual void OnResize() override;
	virtual void Update(const GameTimer& gt) override;
	virtual void Draw(const GameTimer& gt) override;

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {
	
	// 디버그 빌드에서는 실행시점 메모리 점검을 킴
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try {
		InitDirect3DApp theApp(hInstance);
		if (!theApp.Initialize())
			return 0;

		return theApp.Run();
	}
	catch (DxException& e) {
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance)
	: D3DApp(hInstance) { }

InitDirect3DApp::~InitDirect3DApp() { }

bool InitDirect3DApp::Initialize() {
	if (!D3DApp::Initialize())
		return false;

	return true;
}

void InitDirect3DApp::OnResize() {
	D3DApp::OnResize();
}

void InitDirect3DApp::Update(const GameTimer& gt) {

}

void InitDirect3DApp::Draw(const GameTimer& gt) {

	// 명령 기록에 관련된 메모리의 재활용을 위해 명령 할당자를 재설정
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	ThrowIfFailed(mCommandList->Reset(
		mDirectCmdListAlloc.Get(), nullptr));

	// CurrentBackBuffer 를
	// PRESENT(현재 화면) -> RENDER TARGET (렌더링 대상) 으로 변경
	CD3DX12_RESOURCE_BARRIER transitionToRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	mCommandList->ResourceBarrier(1, &transitionToRenderTarget);

	// OnResize 마지막에 설정했던 Viewport, ScissorRect 관련 값을 설정
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// 후면 버퍼와 깊이 버퍼를 지움
	mCommandList->ClearRenderTargetView(
		CurrentBackBufferView(),
		DirectX::Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(
		DepthStencilView(),
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		1.0f, 0, 0, nullptr);

	// 렌더링 결과가 기록될 렌더 대상 버퍼 지정
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = CurrentBackBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = DepthStencilView();
	mCommandList->OMSetRenderTargets(1, &backBufferView, true, &depthStencilView);

	// CurrentBackBuffer를 
	// RENDER TARGET -> PRESENT 로 다시 변경
	CD3DX12_RESOURCE_BARRIER transitionToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	mCommandList->ResourceBarrier(1, &transitionToPresent);

	// 명령 기록 마치기
	ThrowIfFailed(mCommandList->Close());

	// 명령 실행을 위해
	//	명령 목록을 명령 대기열에 추가
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// 후면 버퍼와 전면 버퍼를 교체
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// 이 프레임의 명령이 처리될 때까지 대기 (비효율적임)
	FlushCommandQueue();
}