#include <windows.h>

#include <fstream>
static HWND bhClick;
static HWND bhStop;
static HWND mcH;
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
                if(mcH == 0){
                    MessageBox(hwnd, "Window not found! Check your target.ini file", "Error", MB_OK | MB_ICONWARNING);
                    break;
                }
                if(wParam == BN_CLICKED && (HWND)lParam == bhStop){
                    PostMessageA(mcH,WM_RBUTTONUP,MK_RBUTTON,0);
                    break;
                }
                if(wParam == BN_CLICKED && (HWND)lParam == bhClick){
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
    std::ifstream targetFile;
    char targetName[128];

    targetFile.open("target.ini");
    int check = targetFile.peek();
    if(check == EOF){
        MessageBox(hwnd, "target.ini is empty!", "Error", MB_OK | MB_ICONWARNING | MB_APPLMODAL);
        return 0;
    }
    targetFile.getline(targetName, 128);
    targetFile.close();
    mcH = FindWindowA(NULL, targetName);

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
		
    bhClick = CreateWindow(
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


