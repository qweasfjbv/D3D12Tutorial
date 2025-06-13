//=================================================
//
// 0. Windows Programming
//	
// - Event-Driven 프로그래밍 모형의 기본 구조 구축
// - 기본적인 창 설정 및 띄우기
// - 메시지 루프와 창 프로시저를 통한 입력 처리
// 
//=================================================
#include <Windows.h>


// 주 창의 핸들
// 생성된 창을 식별하는 용도로 사용됨
HWND ghMainWnd = 0;


// Windows 응용 프로그램 초기화에 필요한 코드를 담은 함수.
// 결과를 bool로 반환
bool InitWindowsApp(HINSTANCE instanceHanle, int show);

// 메시지 루프 코드를 담은 함수
int Run();

// 주 창이 받은 사건들을 처리하는 Window Procedure 함수
// LRESULT : 처리 결과
// CALLBACK : 운영체제가 호출해주는 함수 (함수 자체가 스택을 정리)
LRESULT CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


// Win32 의 시작점
// hInstance : 실행중인 프로그램의 인스턴스 핸들
// pCmdLine : 실행 시 전달된 cmd 인자
// nShowCmd : 윈도우를 어떻게 보여줄지 정하는 값. ShowWindow() 로 함수에 전달
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd) {
	
	// 응용 프로그램 주 창 초기화
	if (!InitWindowsApp(hInstance, nShowCmd))
		return 0;

	// 초기화 성공 시 메시지 루프로 진입
	// WM_QUIT 메시지 받을 때까지 계속 돌아감
	return Run();
}


bool InitWindowsApp(HINSTANCE instanceHanle, int show) {
	
	// 1. WNDCLASS 구조체 채우기
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;							// 가로/세로 크기 조정 시 전체 창을 다시 그림
	wc.lpfnWndProc = WndProc;									// CALLBACK 바인딩
	wc.cbClsExtra = 0;											// 클래스 구조에 추가로 예약할 바이트 (보통 0)
	wc.cbWndExtra = 0;											// 윈도우 인스턴스마다 추가로 예약할 바이트 (보통 0)
	wc.hInstance = instanceHanle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);		// 배경 흰색으로 채우기
	wc.lpszMenuName = 0;										// 클래스에 연결할 메뉴 리소스 이름
	wc.lpszClassName = L"BasicWndClass";						// 클래스의 이름

	// 2. WNDCLASS 인스턴스를 Windows에 등록
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"Register Class FAILED", 0, 0);
		return false;
	}

	// 3. CreateWindow 함수로 창 생성하기
	// 성공 -> 창의 핸들값 리턴, 실패 -> 0 리턴

	ghMainWnd = CreateWindow(
		L"BasicWndClass",		// 사용할 클래스 이름 -> 재사용 가능
		L"Win32Basic",			// 창의 제목
		WS_OVERLAPPEDWINDOW,	// 창의 스타일 (기본스타일, 타이틀바, 팝업창 등..)
		CW_USEDEFAULT,			// rect.x
		CW_USEDEFAULT,			// rect.y
		CW_USEDEFAULT,			// rect.width
		CW_USEDEFAULT,			// rect.height
		0,						// 부모 창 핸들
		0,						// 메뉴 핸들
		instanceHanle,			// 응용 프로그램 인스턴스 핸들
		0						// 추가 생성 플래그들 (추가 데이터)
	);

	// 창 생성 실패
	if (ghMainWnd == 0) {
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// 4. 실제로 화면에 띄우는 함수
	ShowWindow(ghMainWnd, show);
	UpdateWindow(ghMainWnd);

	return true;
}

int Run() {
	
	MSG msg = { 0 };
	BOOL bRet = 1;

	// GetMessage 함수는 WM_QUIT 메시지를 받은 경우에만 0 리턴
	// 다만, GetMessage는 메시지가 들어오지 않을 때에는 Sleep 상태
	//while ((bRet = GetMessage(&msg, 0, 0, 0)) != 0) {

	//	// 메시지 수신에서 오류가 있는 경우엔 -1 리턴
	//	if (bRet == -1) {
	//		MessageBox(0, L"GetMessag FAILED", L"Error", MB_OK);
	//	}
	//	else {
	//		TranslateMessage(&msg);		// 키보드입력 -> 문자메시지로 변환
	//		DispatchMessage(&msg);		// 메시지를 WndProc으로 전달
	//	}
	//}

	// PeekMessage는 메시지가 없으면 제어권을 즉시 반환한다.
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);		
			DispatchMessage(&msg);		
		}
		else {
			// 메시지가 없는 경우 다른 작업 처리 가능
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{
		// 왼쪽 마우스 버튼 -> 메시지 박스 표시
	case WM_LBUTTONDOWN:
		MessageBox(0, L"Hello, World", L"Hello", MB_OK);
		return 0;

		// ESC 키가 눌렸으면 응용프로그램창 닫기
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;

		// 파괴 메시지(X 버튼) -> 종료 메시지를 보냄
		// -> 메시지 루프 종료
	case WM_DESTROY:
		PostQuitMessage(0);		// WM_QUIT을 메시지 큐에 삽입
		return 0;
	}

	// 명시적으로 처리하지 않은 다른 메시지들은 기본 창 프로시저에게 넘겨줌
	return DefWindowProc(hwnd, msg, wParam, lParam);
}