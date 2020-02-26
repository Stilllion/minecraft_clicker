#include <windows.h>
#include <string>

static HWND bhStart;
static HWND bhStop;
static HWND mcH;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	// 9 chars - M I N E C R A F T
	char buffer[10];
	
	GetWindowTextA(hwnd,  buffer, 10);
	
	std::string winName (buffer);
	
	if(winName == "Minecraft"){
		mcH = hwnd;
		return false;
	}
	
	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;
        case WM_DESTROY:
            DestroyWindow(hwnd);
        break;
        case WM_COMMAND:
            {
		EnumWindows(EnumWindowsProc, NULL);
				
                if(wParam == BN_CLICKED && (HWND)lParam == bhStop){
                    PostMessageA(mcH,WM_RBUTTONUP,MK_RBUTTON,0);
                    break;
                }
                if(wParam == BN_CLICKED && (HWND)lParam == bhStart){
                    PostMessageA(mcH,WM_RBUTTONDOWN,MK_RBUTTON,0);
                    break;
                }
            }
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;

    HWND hwnd;
    MSG Msg;

    //Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = "WinClass";
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    // Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "WinClass",
        "Clicker",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 100,
        NULL, NULL, hInstance, NULL);
		
    bhStart = CreateWindow(
        "BUTTON",
        "Start",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        10,
        25,
        100,
        20,
        hwnd,
        NULL,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL);

    bhStop = CreateWindow(
        "BUTTON",
        "Stop",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        120,
        25,
        100,
        20,
        hwnd,
        NULL,
        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
        NULL);
    ShowWindow(hwnd, nCmdShow);
    // The Message Loop
    UpdateWindow(hwnd);
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
