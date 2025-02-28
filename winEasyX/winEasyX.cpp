// winEasyX.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "winEasyX.h"
#include "easyx.h"
#include "windows.h"
#include "shellapi.h"
#include "windowsx.h"
#include "cstdlib"
#include "ctime"

#define MAX_LOADSTRING 100
#define MAX_COUNTER 100
#define FRAME_DELAY 2
#define DRAG_DELAY 30
#pragma comment(lib,"MSIMG32.LIB")

class component
{
    public:
        BOOL isClicked = FALSE;//是否被点击
        BOOL isDragging = FALSE;//是否被拖动
        int posX = 600, posY = 400;//位置
        int mouseX = 0, mouseY = 0;//鼠标位置
        int scale = 2;//缩放倍数
        WORD counter = 0;//计数器
        WORD frame = 0;//当前帧
        int delay = FRAME_DELAY;//帧切换间隔
        WORD dragCounter = 0;//拖动计数器
        IMAGE img;//皮肤
        component(LPCTSTR filename)
        {
            loadimage(&img, filename);
        }
};

class pet
{
public:
    BOOL isClicked = FALSE;//是否被点击
    BOOL isDragging = FALSE;//是否被拖动
    BOOL isMoving = FALSE;//是否移动
    BOOL isLeft = FALSE;//是否朝向左侧
    BOOL isAction = FALSE;//是否正在动作
    int state = STATE_NORMAL;//当前状态
    int posX = 600, posY = 400;//位置
    int mouseX = 0, mouseY = 0;//鼠标位置
    int scale = 4;//缩放倍数
    WORD counter = 0;//计数器
    WORD frame = 0;//当前帧
    int delay = FRAME_DELAY;//帧切换间隔
    WORD dragCounter = 0;//拖动计数器
    IMAGE img;//皮肤
    pet(LPCTSTR filename)
    {
        loadimage(&img, filename);
    }
};

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
IMAGE img;
component Reimu(L"Reimu.png");
pet TSC(L"sticks.png");
int posX=600, posY=400;
NOTIFYICONDATA nid;
//HWND hWnd;
HMENU trayMenu;

DWORD frameOfState[] = { 18,8,8,18 };//帧数

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINEASYX, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINEASYX));

    MSG msg;
    //loadimage(&img, L"Reimu.png");

    // 主消息循环:
    while (1)
    {
        //Reimu.counter = (Reimu.counter + 1) % Reimu.delay;
        //if (Reimu.counter == 0)
        //{
        //    Reimu.frame = (Reimu.frame + 1) % 2;
        //}
        while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                //break;
                return (int)msg.wParam;
            }
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    //DestroyMenu(trayMenu);

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINEASYX));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.hbrBackground = (HBRUSH)GetStockBrush(HOLLOW_BRUSH);
    //wcex.hbrBackground = CreateSolidBrush(RGB(100, 100, 100));
    wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_WINEASYX);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中
   //loadimage(&img, L"Reimu.png");

   HWND hWnd = CreateWindowExW(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, szWindowClass, szTitle, WS_POPUP,
      TSC.posX, TSC.posY, SIZE_W * TSC.scale, SIZE_H * TSC.scale, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }


   memset(&nid, 0, sizeof(NOTIFYICONDATA));
   nid.cbSize = sizeof(NOTIFYICONDATA);
   nid.hWnd = hWnd;
   nid.uID = 1;
   nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   nid.uCallbackMessage = WM_TRAYICON;
   nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STICK));
   wcscpy_s(nid.szTip, L"winEasyX");
   Shell_NotifyIcon(NIM_ADD, &nid);


   trayMenu = CreatePopupMenu();
   AppendMenu(trayMenu, MF_STRING, IDM_ABOUT, L"关于");
   AppendMenu(trayMenu, MF_STRING, IDM_EXIT, L"退出");

   //SetLayeredWindowAttributes(hWnd, RGB(255,255,255), 0, LWA_COLORKEY);
   SetLayeredWindowAttributes(hWnd, RGB(0, 255, 0), 0, LWA_COLORKEY);
   //SetLayeredWindowAttributes(hWnd, RGB(100, 100, 100), 0, LWA_COLORKEY);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   SetTimer(hWnd, 1, 16, NULL);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //IMAGE img;
    //loadimage(&img, L"Reimu.png");
    HWND hDlg;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                //DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                SetWindowPos(hDlg, HWND_TOP, 600, 400, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
                ShowWindow(hDlg, SW_SHOW);
                //UpdateWindow(hDlg);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
        {
            //initgraph(600, 400);
        }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            // 下面使用双缓冲绘图
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hbmMem = CreateCompatibleBitmap(hdc, SIZE_W * TSC.scale, SIZE_H * TSC.scale);
            HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

            // 在缓冲区上绘制图片
            //StretchBlt(hdcMem, 0, 0, Reimu.img.getwidth()*Reimu.scale, Reimu.img.getheight()*Reimu.scale, GetImageHDC(&Reimu.img),
            //    Reimu.img.getwidth() * Reimu.frame / 2, 0, Reimu.img.getwidth() / 2, Reimu.img.getheight() / 2, SRCCOPY);
            StretchBlt(hdcMem, 0, 0, SIZE_W * TSC.scale, SIZE_H * TSC.scale, GetImageHDC(&TSC.img),
                SIZE_W * TSC.frame, TSC.state * SIZE_H, SIZE_W, SIZE_H, SRCCOPY);
            //AlphaBlend(hdcMem, 0, 0, Reimu.img.getwidth()*Reimu.scale, Reimu.img.getheight()*Reimu.scale, GetImageHDC(&Reimu.img),
            //    Reimu.img.getwidth() * Reimu.frame / 2, 0, Reimu.img.getwidth()/2, Reimu.img.getheight()/2, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });

            // 复制缓冲区到窗口
            //BitBlt(hdc, 0, 0, Reimu.img.getwidth()*Reimu.scale, Reimu.img.getheight()*Reimu.scale, hdcMem, 0, 0, SRCCOPY);
            BitBlt(hdc, 0, 0, SIZE_W * TSC.scale, SIZE_H * TSC.scale, hdcMem, 0, 0, SRCCOPY);
            //AlphaBlend(hdc, 0, 0, Reimu.img.getwidth() * Reimu.scale, Reimu.img.getheight() * Reimu.scale, hdcMem,
            //    0, 0, Reimu.img.getwidth() * Reimu.scale, Reimu.img.getheight() * Reimu.scale, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });

            // 释放资源
            SelectObject(hdcMem, hbmOld);
            DeleteObject(hbmMem);
            DeleteDC(hdcMem);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        {
            //closegraph();
            KillTimer(hWnd, 1); // 销毁定时器
            DestroyMenu(trayMenu);
            PostQuitMessage(0);
        }
        break;
    case WM_TRAYICON:
        {
            switch(LOWORD(lParam))
            {
                case WM_LBUTTONUP:
                case WM_RBUTTONUP:
                {
                    // 显示右键菜单
                    POINT pt;
                    GetCursorPos(&pt);
                    //HMENU hMenu = CreatePopupMenu();
                    //AppendMenu(hMenu, MF_STRING, IDM_ABOUT, L"关于");
                    //AppendMenu(hMenu, MF_STRING, IDM_EXIT, L"退出");
                    SetForegroundWindow(hWnd);
                    TrackPopupMenu(trayMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
                    //DestroyMenu(hMenu);
                }
                break;
            }
        }
        break;
    case WM_LBUTTONDOWN:
        {
            SetCapture(hWnd);
            if (!TSC.isClicked)
            {
                TSC.mouseX = LOWORD(lParam);
                TSC.mouseY = HIWORD(lParam);
            }
            TSC.isClicked = TRUE;
        }
        break;
    case WM_LBUTTONUP:
        {
            ReleaseCapture();
            if (!TSC.isDragging)
            {
                TSC.isAction = TRUE;
                TSC.state = (std::rand() % 2 ? STATE_ACT1 : STATE_ACT2);
            }
            TSC.isClicked = FALSE;
            TSC.isDragging = FALSE;
            TSC.dragCounter = 0;
        }
        break;
    case WM_RBUTTONUP:
        {
            // 显示右键菜单
            POINT pt;
            GetCursorPos(&pt);
            //HMENU hMenu = CreatePopupMenu();
            //AppendMenu(hMenu, MF_STRING, IDM_ABOUT, L"关于");
            //AppendMenu(hMenu, MF_STRING, IDM_EXIT, L"退出");
            SetForegroundWindow(hWnd);
            TrackPopupMenu(trayMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
            //DestroyMenu(hMenu);
        }
        break;
    case WM_MOUSEMOVE:
        {
            if (TSC.isDragging)
            {
                int x=GET_X_LPARAM(lParam), y=GET_Y_LPARAM(lParam);
                //lParam里是相对窗口左上角的坐标，非相对屏幕区域左上角的坐标，因此不应当提前保存初始位置
                TSC.posX = TSC.posX + GET_X_LPARAM(lParam) - TSC.mouseX;
                TSC.posY = TSC.posY + GET_Y_LPARAM(lParam) - TSC.mouseY;
                SetWindowPos(hWnd, HWND_TOPMOST, TSC.posX, TSC.posY, SIZE_W * TSC.scale, SIZE_H * TSC.scale, SWP_SHOWWINDOW);
                //InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;
    case WM_TIMER:
        {
            TSC.counter = (TSC.counter + 1) % TSC.delay;
            if (TSC.counter == 0)
            {
                //TSC.frame = (TSC.frame + 1) % frameOfState[TSC.state];
                TSC.frame = TSC.frame + 1;
                //UpdateWindow(hWnd);
            }
            if (TSC.isClicked)
            {
                TSC.dragCounter = (TSC.dragCounter + 1) % DRAG_DELAY;
                if (TSC.dragCounter == 0)
                {
                    TSC.isDragging = TRUE;
                }
            }
            if (TSC.isAction)
            {
                TSC.counter = 0;
                TSC.frame = 0;
                TSC.isAction = FALSE;
            }
            InvalidateRect(hWnd, NULL, FALSE);
            if (TSC.frame == frameOfState[TSC.state])
            {
                switch (TSC.state)
                {
                    case STATE_ACT1:
                    case STATE_ACT2:
                        TSC.state = STATE_NORMAL;
                        break;
                    default:
                        break;
                }
                TSC.frame = 0;
            }
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
